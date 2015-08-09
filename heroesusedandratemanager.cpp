#include "heroesusedandratemanager.h"

HeroesUsedAndRateManager::HeroesUsedAndRateManager()
{
}

HeroesUsedAndRate &HeroesUsedAndRateManager::getHeroesUsedAndRate(bool force_download, const DataConfig &config)
{
    static QString keyfmt = "heroesusedandrate%1";
    QString key = keyfmt.arg(config.getFileParams());

    auto i = list.find(key);
    if(i == list.end())
    {
        i = list.insert(key, {});
    }
	i->load(force_download);
    return *i;
}
