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

        list.insert(name, {name, rate, used});
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



float HeroItems::getX2(int heroused, float herorate, const QString &name)
{
    auto i = list.find(name);
    if(i != list.end())
    {
        ItemRateAndUsed &item = *i;

        float a = item.used * item.rate;
        float b = item.used * (1 - item.rate);
        float c = heroused * herorate - item.used * item.rate;
        float d = heroused * (1 - herorate) - item.used * (1 - item.rate);
        float n = heroused;
        float a1 = n * pow(a * d - b * c, 2);
        float a2 = (a + b) * (c + d) * (a + c) * (b + d);
        return a1 / a2;
    }
    else
        return 0;
}

float HeroItems::getX2(int heroused, float herorate, int itemused, float itemrate)
{
    float a = itemused * itemrate;
    float b = itemused * (1 - itemrate);
    float c = heroused * herorate - itemused * itemrate;
    float d = heroused * (1 - herorate) - itemused * (1 - itemrate);
    float n = heroused;
    float a1 = n * pow(a * d - b * c, 2);
    float a2 = (a + b) * (c + d) * (a + c) * (b + d);
    return a1 / a2;
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

        list.insert(name, {name, rate, used});
    }
}

QString HeroItems::getHeroItemsFilename()
{
    auto config = DataConfig::getCurrentConfig();
    QString filename("%1_items%2.xml");
    filename = filename.arg(m_name).arg(config.getFileParams());
    return filename;
}

QString HeroItems::getHeroItemsX2Filename()
{
    auto config = DataConfig::getCurrentConfig();
    QString filename("%1_items_X2%2.xml");
    filename = filename.arg(m_name).arg(config.getFileParams());
    return filename;
}

