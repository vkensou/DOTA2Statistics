#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "Singleton.h"
#include <QtSql>
#include <functional>

class HeroRateAndUsed;
class HeroItems;
class DataConfig;

class DataBaseManager
        :public Singleton<DataBaseManager>
{
public:
    DataBaseManager();

    bool opendb();
    void closedb();

	bool loadHeroesUsedAndRate(std::function<void(const QString &, int, double)> callback, const DataConfig &config);
	void saveHeroesUsedAndRate(std::function<void(std::function<void(const HeroRateAndUsed &)>)> &callback, const DataConfig &config);

    bool loadHeroItems(HeroItems &hero, const DataConfig &config);
    void saveHeroItems(const HeroItems &hero, const DataConfig &config);

private:
    QSqlDatabase db;
};


#endif // DATAMANAGER_H
