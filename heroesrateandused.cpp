#include "heroesrateandused.h"
#include <QString>
#include "utility.h"
#include <QRegExp>
#include <QDomDocument>
#include <functional>
#include <algorithm>

const QString heroesrateandusedfmt = "http://dotamax.com/hero/rate/?time=%1&server=%2&ladder=%3&skill=%4";

void HeroesRateAndUsed::download(const QString &time, const QString &server, const QString &ladder, const QString &skill)
{
    list.clear();
    QUrl url = heroesrateandusedfmt.arg(time, server, ladder, skill);
    auto page = downloadWebPage(url);

    static QRegExp rx("<tbody>.*</tbody>");
    rx.indexIn(page);
    page = rx.cap(0);

    parseWebPageData(page);
}

void HeroesRateAndUsed::load(const QString &filename)
{
    list.clear();

    QFile file(filename);

    if(!file.exists())
    {
        download();
        save(filename);
        return;
    }

    if(!file.open(QIODevice::ReadOnly))
        return;

    QDomDocument doc;
    doc.setContent(&file);

    auto root = doc.documentElement();

    for(auto node = root.firstChildElement("hero"); !node.isNull(); node = node.nextSiblingElement("hero"))
    {
        QString name = node.attribute("name");
        float rate = node.attribute("rate").toFloat();
        int used = node.attribute("used").toInt();

        list.insert(name, {name, rate, used});
    }
}

void HeroesRateAndUsed::save(const QString &filename)
{
    QDomDocument doc;
    auto root = doc.createElement("RatesAndUsed");
    doc.appendChild(root);

    auto func = [&doc, &root](const HeroRateAndUsed &hru)
    {
        auto node = doc.createElement("hero");
        node.setAttribute("name", hru.name);
        node.setAttribute("rate", hru.rate);
        node.setAttribute("used", hru.used);

        root.appendChild(node);
    };
    std::for_each(list.begin(), list.end(), func);

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QTextStream ts(&file);

    doc.save(ts, 4);
}

float HeroesRateAndUsed::getRate(const QString &chinese_name)
{
    auto f = list.find(chinese_name);
    if(f != list.end())
        return (*f).rate;
    else
        return 0.f;
}

int HeroesRateAndUsed::getUsed(const QString &chinese_name)
{
    auto f = list.find(chinese_name);
    if(f != list.end())
        return (*f).used;
    else
        return 0;
}

void HeroesRateAndUsed::parseWebPageData(const QString &data)
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
        name = tdnode.firstChildElement("span").text();

        tdnode = tdnode.nextSiblingElement();
        rate = percentagetoFloat(tdnode.firstChildElement("div").text());

        tdnode = tdnode.nextSiblingElement();
        used = sepNumStrtoInt(tdnode.firstChildElement("div").text());

        list.insert(name, {name, rate, used});
    }
}
