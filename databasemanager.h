#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "Singleton.h"
#include <QtSql>
#include <functional>

class HeroRateAndUsed;
class ItemRateAndUsed;
class DataConfig;

class DataBaseManager
        :public Singleton<DataBaseManager>
{
public:
    DataBaseManager();

    bool opendb();
    void closedb();

	bool loadHeroesUsedAndRate(std::function<void(const QString &, int, double)> callback, const DataConfig &config);
	void saveHeroesUsedAndRate(std::function<void(std::function<void(const HeroRateAndUsed *)>)> &callback, const DataConfig &config);

	bool loadHeroItems(const QString &heroname, std::function<void(const QString &, int, double, double)> callback, const DataConfig &config);
	void saveHeroItems(const QString &heroname, std::function<void(std::function<void(const ItemRateAndUsed *)>)> &callback, const DataConfig &config);

private:
    QSqlDatabase db;
};


#endif // DATAMANAGER_H
