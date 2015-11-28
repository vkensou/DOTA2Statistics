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
#include <QDebug>

HeroesUsedAndRate::HeroesUsedAndRate()
{
	using namespace std::placeholders;
	m_addHero_callback = std::bind(&HeroesUsedAndRate::addHero, this, _1, _2, _3);
	m_enumList = [this](std::function<void(const HeroRateAndUsed *)> &func)->void
	{
		std::for_each(m_list.begin(), m_list.end(), func);
	};
}

HeroesUsedAndRate::~HeroesUsedAndRate()
{
	pointerContainerDeleteAndClear(m_list);
}

void HeroesUsedAndRate::download()
{
	WebDataDownloader::getInstance().downloadHeroesUsedAndRate(m_addHero_callback, DataConfig::getCurrentConfig());
}

void HeroesUsedAndRate::load(bool force_download)
{
	if (!force_download)
	{
		if (!m_list.empty())
			return;

		if (DataBaseManager::getInstance().loadHeroesUsedAndRate(m_addHero_callback, DataConfig::getCurrentConfig()))
			return;
	}

	pointerContainerDeleteAndClear(m_list);
	download();
	save();
}

void HeroesUsedAndRate::save()
{
	DataBaseManager::getInstance().saveHeroesUsedAndRate(m_enumList, DataConfig::getCurrentConfig());
}

float HeroesUsedAndRate::getRate(const QString &chinese_name)
{
	auto data = getHero(chinese_name);
	return data->rate;
}

int HeroesUsedAndRate::getUsed(const QString &chinese_name)
{
	auto data = getHero(chinese_name);
	return data->used;
}

QString HeroesUsedAndRate::getHeroesUsedAndRateFilename()
{
    static const QString filenamefmt("HeroesUsedAndRate%1.xml");
    return filenamefmt.arg(DataConfig::getFileParamsCurrent());
}

void HeroesUsedAndRate::addHero(const QString &name, int used, double rate)
{
	if (m_list.find(name) != m_list.end())
		qDebug() << name;
	else
		m_list.insert(name, new HeroRateAndUsed{ name, used, rate });
}

HeroRateAndUsed * HeroesUsedAndRate::getHero(const QString &chinese_name)
{
	auto f = m_list.find(chinese_name);
	if (f != m_list.end())
		return f.value();
	else
		return nullptr;

}

HeroRateAndUsed::HeroRateAndUsed(const QString &name, int used, double rate) :name(name), used(used), rate(rate)
{
	int a = 0;
	a = 1 + 3;
}

HeroRateAndUsed::~HeroRateAndUsed()
{
	int a = 0;
	a = 1 + 3;
}
