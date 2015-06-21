#include "heroitems.h"
#include "utility.h"
#include <QRegExp>
#include <QDomDocument>
#include <QtMath>
#include "dataconfig.h"

const QString heroitemsfmt = "http://dotamax.com/hero/detail/hero_items/%1/?";

HeroItems::HeroItems(const QString &name)
    :m_name(name)
{
}

void HeroItems::download()
{
    list.clear();

    auto config = DataConfig::getCurrentConfig();
    QUrl url = heroitemsfmt.arg(m_name) + config.getUrlParams();
    auto page = downloadWebPage(url);

    static QRegExp rx("<tbody>.*</tbody>");
    rx.indexIn(page);
    page = rx.cap(0);

    parseWebPageData(page);
}

void HeroItems::load()
{
    list.clear();

    QString filename = getHeroItemsFilename();

    QFile file(filename);

    if(!file.exists())
    {
        download();
        save();
        return;
    }

    if(!file.open(QIODevice::ReadOnly))
        return;

    QDomDocument doc;
    doc.setContent(&file);

    auto root = doc.documentElement();

    for(auto node = root.firstChildElement("item"); !node.isNull(); node = node.nextSiblingElement("item"))
    {
        QString name = node.attribute("name");
        float rate = node.attribute("rate").toFloat();
        int used = node.attribute("used").toInt();

        addItem(name, rate, used);
    }
}

void HeroItems::save()
{
    QDomDocument doc;
    auto root = doc.createElement("HeroItems");
    doc.appendChild(root);

    auto func = [&doc, &root](const ItemRateAndUsed &hru)
    {
        auto node = doc.createElement("item");
        node.setAttribute("name", hru.name);
        node.setAttribute("rate", hru.rate);
        node.setAttribute("used", hru.used);

        root.appendChild(node);
    };
    std::for_each(list.begin(), list.end(), func);

    QString filename = getHeroItemsFilename();

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QTextStream ts(&file);

    doc.save(ts, 4);
}


void HeroItems::calcX2(int heroused, float herorate)
{
    auto func1 = [this, heroused, herorate](ItemRateAndUsed &item)
    {
        item.x2 = getX2(heroused, herorate, item.name);
    };

    std::for_each(list.begin(), list.end(), func1);


    QDomDocument doc;
    auto root = doc.createElement("X2");
    doc.appendChild(root);

    auto func2 = [&doc, &root](const ItemRateAndUsed &hru)
    {
        auto node = doc.createElement("item");
        node.setAttribute("name", hru.name);
        node.setAttribute("x2", hru.x2);

        root.appendChild(node);
    };
    std::for_each(list.begin(), list.end(), func2);

    QString filename = getHeroItemsX2Filename();
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QTextStream ts(&file);

    doc.save(ts, 4);
}

double HeroItems::getX2(int heroused, double herorate, const QString &name)
{
    auto i = list.find(name);
    if(i != list.end())
    {
        ItemRateAndUsed &item = *i;

        return getX2(heroused, herorate, item.used, item.rate);
    }
    else
        return 0;
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

        addItem(name, rate, used);
    }
}

void HeroItems::addItem(const QString &name, float rate, int used)
{
    if(name == "真视宝石" || name == "不朽之守护")
        return;
    list.insert(name, {name, rate, used});
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

