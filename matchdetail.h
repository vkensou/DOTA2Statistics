#pragma once

#include "dataneedcache.h"
#include <QString>
#include <QUrl>

class MatchDetail
	:public DataNeedCache
{
public:
	MatchDetail(int matchid);

private:
	virtual bool loaded() override;
	virtual bool loadFromDataBase() override;
	virtual void download() override;
	virtual void handleData(bool) override;
	virtual void save() override;
	virtual void clear() override;

	QUrl getMatchDetailURL(int matchid);
	void parseMatchDetailData(QString &data);

public:
	int victoryparty{ 0 };
	int duration{ 0 };
	long long int starttime{ 0 };
	int matchid{ 0 };
	int matchseqnum{ 0 };
	int radianttower{ 0 };
	int diretower{ 0 };
	int radiantbarracks{ 0 };
	int direbarracks{ 0 };
	int cluster{ 0 };
	int firstbloodtime{ 0 };
	int lobbytype{ 0 };
	int humanplayer{ 0 };
	int leagueid{ 0 };
	int positivevotes{ 0 };
	int negativevotes{ 0 };
	int gamemode{ 0 };
	int engine{ 0 };
	int radiantgpm{ 0 }, radiantxpm{ 0 }, radiantherodamage{ 0 }, radianttowerdamage{ 0 }, radiantherohealing{ 0 };
	int diregpm{ 0 }, direxpm{ 0 }, direherodamage{ 0 }, diretowerdamage{ 0 }, direherohealing{ 0 };
	struct PlayerDetail
	{
		int accountid{ 0 };
		int heroid{ 0 };
		int item0{ 0 }, item1{ 0 }, item2{ 0 }, item3{ 0 }, item4{ 0 }, item5{ 0 };
		QString unitname;
		int aitem0{ 0 }, aitem1{ 0 }, aitem2{ 0 }, aitem3{ 0 }, aitem4{ 0 }, aitem5{ 0 };
		int kills{ 0 }, deaths{ 0 }, assists{ 0 };
		int level{ 0 };
		int gold{ 0 };
		int lasthits{ 0 }, denies{ 0 };
		int goldpermin{ 0 }, xppermin{ 0 };
		int goldspent{ 0 };
		int herodamage{ 0 }, towerdamage{ 0 };
		int herohealing{ 0 };
		int leaverstatus{ 0 };
		struct AbilityUpgradesDetail
		{
			int ability{ 0 };
			int time{ 0 };
		}abilityupgrades[25];
	};
	PlayerDetail radiantplayers[5];
	PlayerDetail direplayers[5];
	struct PicksAndBans
	{
		bool ispick{ false };	//pick == true, ban == false
		int heroid{ 0 };
		int team{ 0 };
	}pickbanlist[20];
private:
	bool m_init{ false };
};