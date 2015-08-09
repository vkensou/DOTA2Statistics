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

HeroesUsedAndRate::HeroesUsedAndRate()
{
	m_addHero_callback = [this](const QString &name, int used, double rate)->void
	{
		addHero(name, used, rate);
	};
	m_enumList = [this](std::function<void(const HeroRateAndUsed &)> &func)->void
	{
		std::for_each(m_list.begin(), m_list.end(), func);
	};
}

void HeroesUsedAndRate::download()
{
	auto func = [this](const QString &name, int used, double rate)
	{
		addHero(name, used, rate);
	};

	WebDataDownloader::getInstance().downloadHeroesUsedAndRate(func, DataConfig::getCurrentConfig());
}

void HeroesUsedAndRate::load(bool force_download)
{
	auto func = [this](const QString &name, int used, double rate)
	{
		addHero(name, used, rate);
	};

	if (force_download || !DataBaseManager::getInstance().loadHeroesUsedAndRate(func, DataConfig::getCurrentConfig()))
    {
		m_list.clear();
        download();
        save();
    }
}

void HeroesUsedAndRate::save()
{
	DataBaseManager::getInstance().saveHeroesUsedAndRate(m_enumList, DataConfig::getCurrentConfig());
}

float HeroesUsedAndRate::getRate(const QString &chinese_name)
{
    auto f = m_list.find(chinese_name);
    if(f != m_list.end())
        return (*f).rate;
    else
        return 0.f;
}

int HeroesUsedAndRate::getUsed(const QString &chinese_name)
{
    auto f = m_list.find(chinese_name);
    if(f != m_list.end())
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
	m_list.clear();
}

void HeroesUsedAndRate::addHero(const QString &name, int used, double rate)
{
    m_list.insert(name, {name, used, rate});
}

