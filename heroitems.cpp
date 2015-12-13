#pragma execution_character_set("utf-8")

#include "heroitems.h"
#include "utility.h"
#include <QRegExp>
#include <QDomDocument>
#include <QtMath>
#include "dataconfig.h"
#include "databasemanager.h"
#include "heroesusedandratemanager.h"
#include "herolist.h"
#include "statusbarsetter.h"
#include "webdatadownloader.h"
#include <QDebug>

HeroItems::HeroItems(const QString &name)
    :m_name(name)
    ,m_chinese_name(HeroList::getInstance().getChineseNameByName(m_name))
{
	using namespace std::placeholders;
	m_addItem_callback = std::bind(&HeroItems::addItem, this, _1, _2, _3, _4);
	m_enumList = [this](std::function<void(const ItemRateAndUsed *)> &func)->void
	{
		std::for_each(m_list.begin(), m_list.end(), func);
	};
}

void HeroItems::download()
{
	WebDataDownloader::getInstance().downloadHeroItems(m_name, m_addItem_callback, DataConfig::getCurrentConfig());
}

void HeroItems::save()
{
	DataBaseManager::getInstance().saveHeroItems(m_name, m_enumList, DataConfig::getCurrentConfig());
}

void HeroItems::saveasxml()
{
    QDomDocument doc;
    auto root = doc.createElement("X2");
    doc.appendChild(root);

	std::function<void(const ItemRateAndUsed *)> func = [&doc, &root](const ItemRateAndUsed *hru)
    {
        auto node = doc.createElement("item");
        node.setAttribute("name", hru->name);
        node.setAttribute("x2", hru->x2);

        root.appendChild(node);
    };
    for_each_items(func);

    QString filename = getHeroItemsX2Filename();
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QTextStream ts(&file);

    doc.save(ts, 4);
}

int HeroItems::getItemsCount() const
{
    return m_list.count();
}

void HeroItems::for_each_items(std::function<void(ItemRateAndUsed *)> &func)
{
    std::for_each(m_list.begin(), m_list.end(), func);
}

void HeroItems::for_each_items(std::function<void (const ItemRateAndUsed * )> &func) const
{
    std::for_each(m_list.constBegin(), m_list.constEnd(), func);
}

void HeroItems::calcX2(int heroused, float herorate)
{
	StatusBarSeter::setStatusBar("Calculating X2...");

	std::function<void(ItemRateAndUsed *)> func1 = [this, heroused, herorate](ItemRateAndUsed *item)
    {
        item->x2 = getX2(heroused, herorate, item->used, item->rate);
    };
    for_each_items(func1);

	StatusBarSeter::setStatusBar("Calculate complete");
}

double HeroItems::getX2(int heroused, double herorate, int itemused, double itemrate)
{
    if(heroused <= 0 || itemused <= 0)
        return 0.0;

    double a = itemused * itemrate;
    double b = itemused * (1 - itemrate);
    double c = heroused * herorate - itemused * itemrate;
    double d = heroused * (1 - herorate) - itemused * (1 - itemrate);
    return independenttest(a, b, c, d);
}

void HeroItems::addItem(const QString &name, int used, double rate, double x2)
{
    if(name == "真视宝石" || name == "不朽之守护")
        return;

	if (m_list.find(name) != m_list.end())
		qDebug() << name << used << rate;
	else
	{
		m_list.insert(name, new  ItemRateAndUsed(name, used, rate, x2));
	}
}

QString HeroItems::getHeroItemsFilename()
{
    static const QString filenamefmt("%1_items%2.xml");
    return filenamefmt.arg(m_name).arg(DataConfig::getFileParamsCurrent());
}

QString HeroItems::getHeroItemsX2Filename()
{
    static const QString filenamefmt("%1_items_X2%2.xml");
    return filenamefmt.arg(m_name).arg(DataConfig::getFileParamsCurrent());
}

bool HeroItems::loadFromDataBase()
{
	return DataBaseManager::getInstance().loadHeroItems(m_name, m_addItem_callback, DataConfig::getCurrentConfig());
}

void HeroItems::handleData(bool force_download)
{
	HeroesUsedAndRate &hru = HeroesUsedAndRateManager::getInstance().getHeroesUsedAndRate(force_download);
	calcX2(hru.getUsed(m_chinese_name), hru.getRate(m_chinese_name));
}

