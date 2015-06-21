#ifndef HEROITEMSMANAGER_H
#define HEROITEMSMANAGER_H

#include <QHash>
#include <QString>
#include "heroitems.h"
#include "dataconfig.h"

class HeroItemsManager
{
public:
    HeroItemsManager();

    HeroItems & getHeroItems(const QString &name, const DataConfig &config = DataConfig::getCurrentConfig());
private:
    QHash<QString, HeroItems> list;
};

#endif // HEROITEMSMANAGER_H
