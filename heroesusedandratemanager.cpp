#include "heroesusedandratemanager.h"

HeroesUsedAndRateManager::HeroesUsedAndRateManager()
{
}

HeroesUsedAndRate &HeroesUsedAndRateManager::getHeroesUsedAndRate(const DataConfig &config)
{
    static QString keyfmt = "heroesusedandrate%1";
    QString key = keyfmt.arg(config.getFileParams());

    auto i = list.find(key);
    if(i == list.end())
    {
        i = list.insert(key, {});
        (*i).load();
    }
    return *i;
}
