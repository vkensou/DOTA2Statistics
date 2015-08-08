#include "heroitems.h"
#include "utility.h"
#include <QRegExp>
#include <QDomDocument>
#include <QtMath>
#include "dataconfig.h"
#include "datamanager.h"
#include "heroesusedandratemanager.h"
#include "herolist.h"

const QString heroitemsfmt = "http://dotamax.com/hero/detail/hero_items/%1/?";

HeroItems::HeroItems(const QString &name)
    :m_name(name)
    ,m_chinese_name(HeroList::getInstance().getChineseNameByName(m_name))
{
}

void HeroItems::clear()
{
    m_list.clear();
}

void HeroItems::download()
{
    m_list.clear();

    auto config = DataConfig::getCurrentConfig();
    QUrl url = heroitemsfmt.arg(m_name) + config.getUrlParams();
    auto page = downloadWebPage(url);

    static QRegExp rx("<tbody>.*</tbody>");
    rx.indexIn(page);
    page = rx.cap(0);

    parseWebPageData(page);
}

void HeroItems::load(bool force_download)
{
    if(force_download || !DataManager::getInstance().loadHeroItems(*this, DataConfig::getCurrentConfig()))
    {
        download();
        HeroesUsedAndRate &hru = HeroesUsedAndRateManager::getInstance().getHeroesUsedAndRate(force_download);
        calcX2(hru.getUsed(m_chinese_name), hru.getRate(m_chinese_name));
        save();
    }
}

void HeroItems::save()
{
    DataManager::getInstance().saveHeroItems(*this, DataConfig::getCurrentConfig());
}

void HeroItems::saveasxml()
{
    QDomDocument doc;
    auto root = doc.createElement("X2");
    doc.appendChild(root);

    auto func = [&doc, &root](const ItemRateAndUsed &hru)
    {
        auto node = doc.createElement("item");
        node.setAttribute("name", hru.name);
        node.setAttribute("x2", hru.x2);

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

void HeroItems::for_each_items(std::function<void(ItemRateAndUsed &)> func)
{
    std::for_each(m_list.begin(), m_list.end(), func);
}

void HeroItems::for_each_items(std::function<void (const HeroItems::ItemRateAndUsed &)> func) const
{
    std::for_each(m_list.begin(), m_list.end(), func);
}

void HeroItems::calcX2(int heroused, float herorate)
{
    auto func1 = [this, heroused, herorate](ItemRateAndUsed &item)
    {
        item.x2 = getX2(heroused, herorate, item.used, item.rate);
    };
    for_each_items(func1);
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

void HeroItems::parseWebPageData(const QString &data)
{
    QDomDocument doc;
    doc.setContent(data);

    auto root = doc.documentElement();

    for(auto node = root.firstChildElement("tr"); !node.isNull(); node = node.nextSiblingElement())
    {
        QString name;
        float rate;
        int used;

        auto tdnode = node.firstChildElement();
        name = tdnode.firstChildElement("a").text();
        name.replace(" ", "");
        name.replace("\n", "");

        tdnode = tdnode.nextSiblingElement();
        used = sepNumStrtoInt(tdnode.firstChildElement("div").text());

        tdnode = tdnode.nextSiblingElement();
        rate = percentagetoFloat(tdnode.firstChildElement("div").text());

        addItem(name, used, rate, 0);
    }
}

void HeroItems::addItem(const QString &name, int used, double rate, double x2)
{
    if(name == "真视宝石" || name == "不朽之守护")
        return;
    m_list.insert(name, {name, used, rate, x2});
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

