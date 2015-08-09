#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "Singleton.h"
#include <QtSql>

class HeroesUsedAndRate;
class HeroItems;
class DataConfig;

class DataBaseManager
        :public Singleton<DataBaseManager>
{
public:
    DataBaseManager();

    bool opendb();
    void closedb();

    bool loadHeroesUsedAndRate(HeroesUsedAndRate &hur, const DataConfig &config);
    void saveHeroesUsedAndRate(const HeroesUsedAndRate &hur, const DataConfig &config);

    bool loadHeroItems(HeroItems &hero, const DataConfig &config);
    void saveHeroItems(const HeroItems &hero, const DataConfig &config);

private:
    QSqlDatabase db;
};


#endif // DATAMANAGER_H
