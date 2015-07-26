#ifndef HEROESUSEDANDRATEMANAGER_H
#define HEROESUSEDANDRATEMANAGER_H

#include <QHash>
#include <QString>
#include "heroesusedandrate.h"
#include "dataconfig.h"
#include "Singleton.h"

class HeroesUsedAndRateManager
        :public Singleton<HeroesUsedAndRateManager>
{
public:
    HeroesUsedAndRateManager();

    HeroesUsedAndRate & getHeroesUsedAndRate(bool force_download = false, const DataConfig &config = DataConfig::getCurrentConfig());
private:
    QHash<QString, HeroesUsedAndRate> list;
};


#endif // HEROESUSEDANDRATEMANAGER_H
