#include "HeroesUsedAndRate.h"
#include <QString>
#include "utility.h"
#include <QRegExp>
#include <QDomDocument>
#include <functional>
#include <algorithm>
#include "dataconfig.h"
#include "databasemanager.h"
#include "statusbarsetter.h"
#include "webdatadownloader.h"

void HeroesUsedAndRate::download()
{
	WebDataDownloader::getInstance().downloadHeroesUsedAndRate(*this, DataConfig::getCurrentConfig());
}

void HeroesUsedAndRate::load(bool force_download)
{
    if(force_download || !DataBaseManager::getInstance().loadHeroesUsedAndRate(*this, DataConfig::getCurrentConfig()))
    {
        download();
        save();
    }
}

void HeroesUsedAndRate::save()
{
    DataBaseManager::getInstance().saveHeroesUsedAndRate(*this, DataConfig::getCurrentConfig());
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

QString HeroesUsedAndRate::getHeroesUsedAndRateFilename()
{
    static const QString filenamefmt("HeroesUsedAndRate%1.xml");
    return filenamefmt.arg(DataConfig::getFileParamsCurrent());
}

void HeroesUsedAndRate::clear()
{
	list.clear();
}

void HeroesUsedAndRate::addHero(const QString &name, int used, double rate)
{
    list.insert(name, {name, used, rate});
}

