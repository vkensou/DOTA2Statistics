#ifndef HEROITEMSMANAGER_H
#define HEROITEMSMANAGER_H

#include <QHash>
#include <QString>
#include "heroitems.h"
#include "dataconfig.h"
#include "Singleton.h"
#include "datamanagerbase.h"
class HeroItemsManager
	:public Singleton<HeroItemsManager>, public DataManagerBase<HeroItems>
{
public:
    HeroItems & getHeroItems(const QString &name, const DataConfig &config = DataConfig::getCurrentConfig());

};

#endif // HEROITEMSMANAGER_H
