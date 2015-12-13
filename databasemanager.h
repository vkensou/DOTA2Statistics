#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "Singleton.h"
#include <QtSql>
#include <functional>

class HeroRateAndUsed;
class ItemRateAndUsed;
class DataConfig;
class MatchDetail;

class DataBaseManager
        :public Singleton<DataBaseManager>
{
public:
    DataBaseManager();

    bool opendb();
    void closedb();

	bool loadHeroesUsedAndRate(std::function<void(const QString &, int, double)> &callback, const DataConfig &config);
	void saveHeroesUsedAndRate(std::function<void(std::function<void(const HeroRateAndUsed *)>)> &callback, const DataConfig &config);

	bool loadHeroItems(const QString &heroname, std::function<void(const QString &, int, double, double)> &callback, const DataConfig &config);
	void saveHeroItems(const QString &heroname, std::function<void(std::function<void(const ItemRateAndUsed *)>)> &callback, const DataConfig &config);

	bool loadMatchDetail(MatchDetail& matchdetail);
	void saveMatchDetail(MatchDetail& matchdetail);

	bool isMatchSaved(int matchid);
	bool isPlayerSaved(int playerid);
	void insertPlayerInfo(int playerid);
	void updataPlayerStatus(int playerid, int newstatus);

	int getPlayerRandomly();

	bool joinOtherDatabase(const QString &otherdbpath);

private:
	void initdb();
	void initMatchDetaildbs();
	void initPlayerInfodbs();

	QString getTable_HeroesUsedAndRate_Name(const DataConfig &config);
	QString getTable_HeroItems_Name(const QString &heroname, const DataConfig &config);

	bool loadMatchDetailSide(MatchDetail &matchdetail);
	bool loadMatchDetailPickBanList(MatchDetail &matchdetail);
	bool loadMatchDetailPlayerInfo(MatchDetail &matchdetail);
	bool loadMatchDetailPlayerAbilitiesUpgragde(MatchDetail &matchdetail);

	void saveMatchDetailSide(MatchDetail &matchdetail);
	void saveMatchDetailPickBanList(MatchDetail &matchdetail);
	void saveMatchDetailPlayerInfo(MatchDetail &matchdetail);
	void saveMatchDetailPlayerAbilitiesUpgragde(MatchDetail &matchdetail);

private:
    QSqlDatabase db;
};


#endif // DATAMANAGER_H
