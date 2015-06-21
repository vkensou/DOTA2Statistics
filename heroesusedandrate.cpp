#include "HeroesUsedAndRate.h"
#include <QString>
#include "utility.h"
#include <QRegExp>
#include <QDomDocument>
#include <functional>
#include <algorithm>
#include "dataconfig.h"
#include "datamanager.h"

void HeroesUsedAndRate::download()
{
    list.clear();

    QUrl url = getHeroesUsedAndRateUrl();
    auto page = downloadWebPage(url);

    static QRegExp rx("<tbody>.*</tbody>");
    rx.indexIn(page);
    page = rx.cap(0);

    parseWebPageData(page);
}

void HeroesUsedAndRate::load(bool force_download)
{
    if(force_download || !DataManager::getInstance().loadHeroesUsedAndRate(*this, DataConfig::getCurrentConfig()))
    {
        download();
        save();
    }
}

void HeroesUsedAndRate::save()
{
    DataManager::getInstance().saveHeroesUsedAndRate(*this, DataConfig::getCurrentConfig());
}

float HeroesUsedAndRate::getRate(const QString &chinese_name)
{
    auto f = list.find(chinese_name);
    if(f != list.end())
        return (*f).rate;
    else
        return 0.f;
}

int HeroesUsedAndRate::getUsed(const QString &chinese_name)
{
    auto f = list.find(chinese_name);
    if(f != list.end())
        return (*f).used;
    else
        return 0;
}

void HeroesUsedAndRate::parseWebPageData(const QString &data)
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

        addHero(name, used, rate);
    }
}

QUrl HeroesUsedAndRate::getHeroesUsedAndRateUrl()
{
    static const QString urlfmt = "http://dotamax.com/hero/rate/?";
    return urlfmt + DataConfig::getUrlParamsCurrent();
}

QString HeroesUsedAndRate::getHeroesUsedAndRateFilename()
{
    static const QString filenamefmt("HeroesUsedAndRate%1.xml");
    return filenamefmt.arg(DataConfig::getFileParamsCurrent());
}

void HeroesUsedAndRate::addHero(const QString &name, int used, double rate)
{
    list.insert(name, {name, used, rate});
}

