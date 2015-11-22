#include "heroesusedandratemanager.h"
#include "utility.h"

HeroesUsedAndRateManager::HeroesUsedAndRateManager()
{
}

HeroesUsedAndRateManager::~HeroesUsedAndRateManager()
{
	pointerContainerDeleteAndClear(m_list);
}

HeroesUsedAndRate &HeroesUsedAndRateManager::getHeroesUsedAndRate(bool force_download, const DataConfig &config)
{
    static QString keyfmt = "heroesusedandrate%1";
    QString key = keyfmt.arg(config.getFileParams());

    auto i = m_list.find(key);
	if (i == m_list.end())
    {
		i = m_list.insert(key, new HeroesUsedAndRate);
    }
	HeroesUsedAndRate * data = i.value();
	data->load(force_download);
    return *data;
}
