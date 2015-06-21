#ifndef HEROESUSEDANDRATEMANAGER_H
#define HEROESUSEDANDRATEMANAGER_H

#include <QHash>
#include <QString>
#include "heroesusedandrate.h"
#include "dataconfig.h"

class HeroesUsedAndRateManager
{
public:
    HeroesUsedAndRateManager();

    HeroesUsedAndRate & getHeroesUsedAndRate(const DataConfig &config = DataConfig::getCurrentConfig());
private:
    QHash<QString, HeroesUsedAndRate> list;
};


#endif // HEROESUSEDANDRATEMANAGER_H
