#ifndef HEROESUSEDANDRATEMANAGER_H
#define HEROESUSEDANDRATEMANAGER_H

#include <QHash>
#include <QString>
#include "heroesusedandrate.h"
#include "dataconfig.h"
#include "Singleton.h"
#include "datamanagerbase.h"

class HeroesUsedAndRateManager
	:public Singleton<HeroesUsedAndRateManager>, public DataManagerBase<HeroesUsedAndRate>
{
public:
    HeroesUsedAndRate & getHeroesUsedAndRate(bool force_download = false, const DataConfig &config = DataConfig::getCurrentConfig());

};


#endif // HEROESUSEDANDRATEMANAGER_H
