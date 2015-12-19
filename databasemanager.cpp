#include "databasemanager.h"
#include "dataconfig.h"
#include "heroesusedandrate.h"
#include "heroitems.h"
#include <functional>
#include <algorithm>
#include "matchdetail.h"
#include <QDir>

DataBaseManager::DataBaseManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dota2satistics.db");

}

bool DataBaseManager::opendb()
{
	if (!db.open())
		return false;

	initdb();
	return true;
}

void DataBaseManager::closedb()
{
    db.close();
}

bool DataBaseManager::loadHeroesUsedAndRate(std::function<void(const QString &, int, double)> &callback, const DataConfig &config)
{
	QString tablename = getTable_HeroesUsedAndRate_Name(config);

    QSqlTableModel model(0, db);
    model.setTable(tablename);
    model.select();

	QSqlError error = model.lastError();
	QString errortext = error.text();

	if (model.rowCount() == 0)
        return false;
    else
    {
        for(int i = 0; i < model.rowCount(); ++i)
        {
            auto record = model.record(i);
            auto name = record.value("name").toString();
            auto used = record.value("used").toInt();
            auto rate = record.value("rate").toDouble();
			callback(name, used, rate);
        }
        return true;
    }
}

void DataBaseManager::saveHeroesUsedAndRate(std::function<void(std::function<void(const HeroRateAndUsed *)>)> &callback, const DataConfig &config)
{
	QString tablename = "[" + getTable_HeroesUsedAndRate_Name(config) + "]";

    static QString sqlcreate = "CREATE TABLE IF NOT EXISTS %1 (name TEXT NOT NULL, used INTEGER NOT NULL, rate DOUBLE NOT NULL);";
    static QString sqldelete = "DELETE from %1;";
    static QString sqlinsert = "INSERT INTO %1(name, used, rate) VALUES('%2', %3, %4);";

    db.exec(sqlcreate.arg(tablename));
    db.exec(sqldelete.arg(tablename));

    db.transaction();
	auto func = [this, &tablename](const HeroRateAndUsed *hero)
    {
		QString sql = sqlinsert.arg(tablename).arg(hero->name).arg(hero->used).arg(hero->rate);
        db.exec(sql);
    };
	callback(func);
    db.commit();

	QSqlError error = db.lastError();
    QString errortext = error.text();
}

bool DataBaseManager::loadHeroItems(const QString &heroname, std::function<void(const QString &, int, double, double)> &callback, const DataConfig &config)
{
	QString tablename = getTable_HeroItems_Name(heroname, config);

    QSqlTableModel model(0, db);
    model.setTable(tablename);
    model.select();

	QSqlError error = model.lastError();
	QString errortext = error.text();

	if (model.rowCount() == 0)
        return false;
    else
    {
        for(int i = 0; i < model.rowCount(); ++i)
        {
            auto record = model.record(i);
            auto name = record.value("name").toString();
            auto used = record.value("used").toInt();
            auto rate = record.value("rate").toDouble();
            auto x2 = record.value("x2").toDouble();
			callback(name, used, rate, x2);
        }
        return true;
    }
}

void DataBaseManager::saveHeroItems(const QString &heroname, std::function<void(std::function<void(const ItemRateAndUsed *)>)> &callback, const DataConfig &config)
{
	QString tablename = "[" + getTable_HeroItems_Name(heroname, config) + "]";

	static QString sqlcreate = "CREATE TABLE IF NOT EXISTS %1 (name TEXT NOT NULL, used INTEGER NOT NULL, rate DOUBLE NOT NULL, x2 DOUBLE NOT NULL);";
    static QString sqldelete = "DELETE from %1;";
    static QString sqlinsert = "INSERT INTO %1(name, used, rate, x2) VALUES('%2', %3, %4, %5);";

    db.exec(sqlcreate.arg(tablename));
    db.exec(sqldelete.arg(tablename));

    db.transaction();
	auto func = [this, &tablename](const ItemRateAndUsed * item)
    {
		QString sql = sqlinsert.arg(tablename).arg(item->name).arg(item->used).arg(item->rate).arg(item->x2);
        db.exec(sql);
    };
	callback(func);
    db.commit();

	QSqlError error = db.lastError();
	QString errortext = error.text();
}

bool DataBaseManager::loadMatchDetail(MatchDetail& matchdetail)
{
	static QString tablename = "matchdetail";

	QSqlTableModel model(0, db);
	model.setTable(tablename);
	model.setFilter(QString("matchid=%1").arg(matchdetail.matchid));
	model.select();

	if (model.rowCount() == 0)
		return false;
	else
	{
		auto record = model.record(0);
		matchdetail.victoryparty = record.value("victoryparty").toInt();
		matchdetail.duration = record.value("duration").toInt();
		matchdetail.matchseqnum = record.value("seqnum").toInt();
		matchdetail.cluster = record.value("cluster").toInt();
		matchdetail.firstbloodtime = record.value("firstbloodtime").toInt();
		matchdetail.lobbytype = record.value("lobbytype").toInt();
		matchdetail.humanplayer = record.value("humanplayer").toInt();
		matchdetail.leagueid = record.value("leagueid").toInt();
		matchdetail.positivevotes = record.value("positivevotes").toInt();
		matchdetail.negativevotes = record.value("negativevotes").toInt();
		matchdetail.gamemode = record.value("gamemode").toInt();
		matchdetail.engine = record.value("engine").toInt();
		matchdetail.starttime = record.value("starttime").toInt();
	}

	if (!loadMatchDetailSide(matchdetail))
		return false;

	if (matchdetail.lobbytype == 2 && !loadMatchDetailPickBanList(matchdetail))
		return false;

	return loadMatchDetailPlayerInfo(matchdetail);
}

void DataBaseManager::saveMatchDetail(MatchDetail &matchdetail)
{
	static QString tablename = "matchdetail";

	QSqlTableModel model(0, db);
	model.setTable(tablename);
	model.setFilter(QString("matchid=%1").arg(matchdetail.matchid));
	model.select();

	if (model.rowCount() != 0)
		return;

	static QString sqlinsert = "INSERT INTO matchdetail(matchid, victoryparty, duration, seqnum, cluster, firstbloodtime, lobbytype, humanplayer, leagueid, "
		"positivevotes, negativevotes, gamemode, engine, starttime) "
		"VALUES(%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14);";

	db.exec(sqlinsert.arg(matchdetail.matchid).arg(matchdetail.victoryparty).arg(matchdetail.duration).arg(matchdetail.matchseqnum).arg(matchdetail.cluster)
		.arg(matchdetail.firstbloodtime).arg(matchdetail.lobbytype).arg(matchdetail.humanplayer).arg(matchdetail.leagueid).arg(matchdetail.positivevotes)
		.arg(matchdetail.negativevotes).arg(matchdetail.gamemode).arg(matchdetail.engine).arg(matchdetail.starttime)
		);
	auto e2 = db.lastError().text();

	saveMatchDetailSide(matchdetail);

	if (matchdetail.gamemode == 2)
		saveMatchDetailPickBanList(matchdetail);

	saveMatchDetailPlayerInfo(matchdetail);
}

bool DataBaseManager::isMatchSaved(int matchid)
{
	QSqlTableModel model(0, db);
	model.setTable("matchdetail");
	model.setFilter(QString("matchid=%1").arg(matchid));
	model.select();

	return model.rowCount() != 0;
}

bool DataBaseManager::isPlayerSaved(int playerid)
{
	QSqlTableModel model(0, db);
	model.setTable("playerinfo");
	model.setFilter(QString("accountid=%1").arg(playerid));
	model.select();

	return model.rowCount() != 0;
}

void DataBaseManager::insertPlayerInfo(int playerid)
{
	QSqlTableModel model(0, db);
	model.setTable("playerinfo");
	model.setFilter(QString("accountid=%1").arg(playerid));
	model.select();
	if (model.rowCount() == 0)
	{
		QSqlRecord record = model.record();
		record.setValue("accountid", playerid);
		record.setValue("status", 0);
		model.insertRecord(0, record);
		model.submit();
	}
}

void DataBaseManager::updataPlayerStatus(int playerid, int newstatus)
{
	QSqlTableModel model(0, db);
	model.setTable("playerinfo");
	model.setFilter(QString("accountid=%1").arg(playerid));
	model.select();
	if (model.rowCount() == 1)
	{
		QSqlRecord record = model.record(0);
		record.setValue("status", newstatus);
		model.setRecord(0, record);
		model.submit();
	}
}

int DataBaseManager::getPlayerRandomly()
{
	static QString sqlrandomselect = "select accountid from matchdetail_playerinfo ORDER BY random() LIMIT 1;";
	auto query = db.exec(sqlrandomselect);
	if (query.size() == 0)
		return 0;

	query.first();
	return query.value(0).toInt();
}

bool DataBaseManager::joinOtherDatabase(const QString &otherdbpath)
{
	QString newdbpath = otherdbpath.toLower();
	QString dbpath = QDir::currentPath() + "/" + "dota2satistics.db";
	dbpath = dbpath.toLower();
	if (dbpath == newdbpath)
		return false;

	QSqlDatabase newdb = QSqlDatabase::addDatabase("QSQLITE", "joineddb");
	newdb.setDatabaseName(newdbpath);
	if (!newdb.open())
		return false;

	QSqlTableModel modelnew(0, newdb);
	modelnew.setTable("matchdetail");
	modelnew.select();

	if (modelnew.rowCount() == 0)
	{
		newdb.close();
		return true;
	}

	db.transaction();

	for (int i = 0; i < modelnew.rowCount(); ++i)
	{
		auto record = modelnew.record(i);
		int matchid = record.value("matchid").toInt();
		static QString sqlselect = "select matchid from matchdetail WHERE matchid = %1;";
		QSqlQuery r = db.exec(sqlselect.arg(matchid));
		//r.next()返回true，说明不为空
		if (r.next())
			continue;
		MatchDetail matchdetail(matchid);
		//load
		//match detail
		matchdetail.victoryparty = record.value("victoryparty").toInt();
		matchdetail.duration = record.value("duration").toInt();
		matchdetail.matchseqnum = record.value("seqnum").toInt();
		matchdetail.cluster = record.value("cluster").toInt();
		matchdetail.firstbloodtime = record.value("firstbloodtime").toInt();
		matchdetail.lobbytype = record.value("lobbytype").toInt();
		matchdetail.humanplayer = record.value("humanplayer").toInt();
		matchdetail.leagueid = record.value("leagueid").toInt();
		matchdetail.positivevotes = record.value("positivevotes").toInt();
		matchdetail.negativevotes = record.value("negativevotes").toInt();
		matchdetail.gamemode = record.value("gamemode").toInt();
		matchdetail.engine = record.value("engine").toInt();
		matchdetail.starttime = record.value("starttime").toInt();
		//side
		{
			static QString tablename = "matchdetail_side";

			QSqlTableModel model(0, newdb);
			model.setTable(tablename);
			model.setFilter(QString("matchid=%1").arg(matchdetail.matchid));
			model.select();

			if (model.rowCount() != 2)
				return false;
			else
			{
				for (int i = 0; i < 2; ++i)
				{
					QSqlRecord record;
					int side;

					record = model.record(i);
					side = record.value("side").toInt();
					if (side == 1)
					{
						matchdetail.radianttower = record.value("tower").toInt();
						matchdetail.radiantbarracks = record.value("barrack").toInt();
						matchdetail.radiantgpm = record.value("gpm").toInt();
						matchdetail.radiantxpm = record.value("xpm").toInt();
						matchdetail.radiantherodamage = record.value("herodamage").toInt();
						matchdetail.radianttowerdamage = record.value("towerdamage").toInt();
						matchdetail.radiantherohealing = record.value("herohealing").toInt();
					}
					else
					{
						matchdetail.diretower = record.value("tower").toInt();
						matchdetail.direbarracks = record.value("barrack").toInt();
						matchdetail.diregpm = record.value("gpm").toInt();
						matchdetail.direxpm = record.value("xpm").toInt();
						matchdetail.direherodamage = record.value("herodamage").toInt();
						matchdetail.diretowerdamage = record.value("towerdamage").toInt();
						matchdetail.direherohealing = record.value("herohealing").toInt();
					}
				}
			}
		}
		//banpick
		if (matchdetail.lobbytype == 2)
		{
			static QString tablename = "matchdetail_pickban";

			QSqlTableModel model(0, newdb);
			model.setTable(tablename);
			model.setFilter(QString("matchid=%1").arg(matchdetail.matchid));
			model.select();

			if (model.rowCount() == 0)
				return false;
			else
			{
				for (int i = 0; i < model.rowCount(); ++i)
				{
					auto record = model.record(i);
					int order = record.value("bporder").toInt();
					matchdetail.pickbanlist[order].ispick = record.value("ispick").toInt() == 1;
					matchdetail.pickbanlist[order].heroid = record.value("heroid").toInt();
					matchdetail.pickbanlist[order].team = record.value("team").toInt();
				}
			}
		}
		//playerinfo
		{
			static QString tablename = "matchdetail_playerinfo";

			QSqlTableModel model(0, newdb);
			model.setTable(tablename);
			model.setFilter(QString("matchid=%1").arg(matchdetail.matchid));
			model.select();

			if (model.rowCount() == 0)
				return false;
			else
			{
				for (int i = 0; i < model.rowCount(); ++i)
				{
					auto record = model.record(i);
					int slot = record.value("slot").toInt();

					MatchDetail::PlayerDetail *player;
					if (slot < 128)
						player = &matchdetail.radiantplayers[slot];
					else
						player = &matchdetail.direplayers[slot - 128];

					player->accountid = record.value("accountid").toInt();
					player->heroid = record.value("heroid").toInt();
					player->unitname = record.value("unitname").toString();
					player->kills = record.value("kills").toInt();
					player->deaths = record.value("deaths").toInt();
					player->assists = record.value("assists").toInt();
					player->level = record.value("level").toInt();
					player->gold = record.value("gold").toInt();
					player->lasthits = record.value("lasthits").toInt();
					player->denies = record.value("denies").toInt();
					player->goldpermin = record.value("goldpermin").toInt();
					player->xppermin = record.value("xppermin").toInt();
					player->goldspent = record.value("goldspent").toInt();
					player->herodamage = record.value("herodamage").toInt();
					player->towerdamage = record.value("towerdamage").toInt();
					player->herohealing = record.value("herohealing").toInt();
					player->leaverstatus = record.value("leaverstatus").toInt();
				}
			}
		}
		//abilities
		{
			static QString tablename = "matchdetail_abilitiesupgrade";

			QSqlTableModel model(0, newdb);
			model.setTable(tablename);
			model.setFilter(QString("matchid=%1").arg(matchdetail.matchid));
			model.select();

			if (model.rowCount() == 0)
				return false;
			else
			{
				for (int i = 0; i < model.rowCount(); ++i)
				{
					auto record = model.record(i);
					int slot = record.value("slot").toInt();

					MatchDetail::PlayerDetail *player;
					if (slot < 128)
						player = &matchdetail.radiantplayers[slot];
					else
						player = &matchdetail.direplayers[slot - 128];

					int level = record.value("level").toInt();
					MatchDetail::PlayerDetail::AbilityUpgradesDetail &ability = player->abilityupgrades[level];
					ability.ability = record.value("ability").toInt();
					ability.time = record.value("time").toInt();
				}
			}
		}
		//save
		{
			static QString sqlinsertmatch = "INSERT INTO matchdetail(matchid, victoryparty, duration, seqnum, cluster, firstbloodtime, lobbytype, humanplayer, leagueid, "
				"positivevotes, negativevotes, gamemode, engine, starttime) "
				"VALUES(%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14);";

			db.exec(sqlinsertmatch.arg(matchdetail.matchid).arg(matchdetail.victoryparty).arg(matchdetail.duration).arg(matchdetail.matchseqnum).arg(matchdetail.cluster)
				.arg(matchdetail.firstbloodtime).arg(matchdetail.lobbytype).arg(matchdetail.humanplayer).arg(matchdetail.leagueid).arg(matchdetail.positivevotes)
				.arg(matchdetail.negativevotes).arg(matchdetail.gamemode).arg(matchdetail.engine).arg(matchdetail.starttime)
				);

			static QString sqlinsertpickban = "INSERT INTO matchdetail_pickban(matchid, ispick, heroid, team, bporder) VALUES(%1, %2, %3, %4, %5);";
			for (int i = 0; i < 20; ++i)
			{
				db.exec(sqlinsertpickban.arg(matchdetail.matchid).arg(matchdetail.pickbanlist[i].ispick ? 1 : 0).arg(matchdetail.pickbanlist[i].heroid).arg(matchdetail.pickbanlist[i].team).arg(i));
			}

			static QString sqlinsertplayer = "INSERT INTO matchdetail_playerinfo(matchid, accountid, heroid, slot, unitname"
				", kills, deaths, assists, level, gold, lasthits, denies, goldpermin, xppermin, goldspent, herodamage, towerdamage, herohealing, leaverstatus) "
				"VALUES(%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14, %15, %16, %17, %18, %19);";
			for (int i = 0; i < 5; ++i)
			{
				MatchDetail::PlayerDetail &player = matchdetail.radiantplayers[i];
				db.exec(sqlinsertplayer.arg(matchdetail.matchid).arg(player.accountid).arg(player.heroid).arg(i)
					.arg("'" + player.unitname + "'").arg(player.kills).arg(player.deaths).arg(player.assists)
					.arg(player.level).arg(player.gold).arg(player.lasthits).arg(player.denies).arg(player.goldpermin).arg(player.xppermin).arg(player.goldspent).arg(player.herodamage).arg(player.towerdamage).arg(player.herohealing).arg(player.leaverstatus));
			}
			for (int i = 0; i < 5; ++i)
			{
				MatchDetail::PlayerDetail &player = matchdetail.direplayers[i];
				db.exec(sqlinsertplayer.arg(matchdetail.matchid).arg(player.accountid).arg(player.heroid).arg(i + 128)
					.arg("'" + player.unitname + "'").arg(player.kills).arg(player.deaths).arg(player.assists)
					.arg(player.level).arg(player.gold).arg(player.lasthits).arg(player.denies).arg(player.goldpermin).arg(player.xppermin).arg(player.goldspent).arg(player.herodamage).arg(player.towerdamage).arg(player.herohealing).arg(player.leaverstatus));
			}

			static QString sqlinsertabilities = "INSERT INTO matchdetail_abilitiesupgrade(matchid, slot, ability, time, level) "
				"VALUES(%1, %2, %3, %4, %5);";
			for (int i = 0; i < 5; ++i)
			{
				MatchDetail::PlayerDetail &player = matchdetail.radiantplayers[i];
				for (int j = 0; j < 25; ++j)
				{
					MatchDetail::PlayerDetail::AbilityUpgradesDetail &abilities = player.abilityupgrades[j];
					if (abilities.ability == 0)
						break;
					db.exec(sqlinsertabilities.arg(matchdetail.matchid).arg(i).arg(abilities.ability).arg(abilities.time).arg(j));
				}
			}
			for (int i = 0; i < 5; ++i)
			{
				MatchDetail::PlayerDetail &player = matchdetail.direplayers[i];
				for (int j = 0; j < 25; ++j)
				{
					MatchDetail::PlayerDetail::AbilityUpgradesDetail &abilities = player.abilityupgrades[j];
					if (abilities.ability == 0)
						break;
					db.exec(sqlinsertabilities.arg(matchdetail.matchid).arg(i + 128).arg(abilities.ability).arg(abilities.time).arg(j));
				}
			}

			static QString sqlinsertside = "INSERT INTO matchdetail_side(matchid, side, win, tower, barrack, gpm, xpm, herodamage, towerdamage, herohealing) VALUES(%1, %2, %3, %4, %5, %6, %7, %8, %9, %10);";
			db.exec(sqlinsertside.arg(matchdetail.matchid).arg(1).arg(matchdetail.victoryparty == 1 ? 1 : 0).arg(matchdetail.radianttower).arg(matchdetail.radiantbarracks).arg(matchdetail.radiantgpm).arg(matchdetail.radiantxpm).arg(matchdetail.radiantherodamage).arg(matchdetail.radianttowerdamage).arg(matchdetail.radiantherohealing));
			db.exec(sqlinsertside.arg(matchdetail.matchid).arg(0).arg(matchdetail.victoryparty == 0 ? 1 : 0).arg(matchdetail.diretower).arg(matchdetail.direbarracks).arg(matchdetail.diregpm).arg(matchdetail.direxpm).arg(matchdetail.direherodamage).arg(matchdetail.diretowerdamage).arg(matchdetail.direherohealing));
		}
	}

	if (!db.commit())
	{
		QString errorstr = db.lastError().text();
	}

	newdb.close();
	return true;
}

void DataBaseManager::initdb()
{
	initMatchDetaildbs();
	initPlayerInfodbs();
}

void DataBaseManager::initMatchDetaildbs()
{
	static QString sqlcreatematchdetail = "CREATE TABLE IF NOT EXISTS matchdetail(matchid  INTEGER NOT NULL,"
		"victoryparty  INTEGER NOT NULL,"
		"duration  INTEGER NOT NULL,"
		"seqnum  INTEGER NOT NULL,"
		"cluster  INTEGER NOT NULL,"
		"firstbloodtime  INTEGER NOT NULL,"
		"lobbytype  INTEGER NOT NULL,"
		"humanplayer  INTEGER NOT NULL,"
		"leagueid  INTEGER NOT NULL DEFAULT 0,"
		"positivevotes  INTEGER NOT NULL,"
		"negativevotes  INTEGER NOT NULL,"
		"gamemode  INTEGER NOT NULL,"
		"engine  INTEGER NOT NULL,"
		"starttime  INTEGER NOT NULL,"
		"PRIMARY KEY(matchid)); ";

	db.exec(sqlcreatematchdetail);

	static QString sqlcreatematchdetailpickban = "CREATE TABLE IF NOT EXISTS matchdetail_pickban("
		"matchid  INTEGER NOT NULL,"
		"ispick  INTEGER NOT NULL,"
		"heroid  INTEGER NOT NULL,"
		"team  INTEGER NOT NULL,"
		"bporder  INTEGER NOT NULL,"
		"FOREIGN KEY(matchid) REFERENCES matchdetail (matchid),"
		");";

	db.exec(sqlcreatematchdetailpickban);

	static QString sqlindexmatchdetailpickban = "CREATE INDEX IF NOT EXISTS index_matchid ON matchdetail_pickban("
		"matchid)";

	db.exec(sqlindexmatchdetailpickban);

	static QString sqlcreatematchdetail_playerinfo = "CREATE TABLE IF NOT EXISTS matchdetail_playerinfo("
		"matchid  INTEGER NOT NULL,"
		"accountid  INTEGER NOT NULL,"
		"heroid  INTEGER NOT NULL,"
		"slot  INTEGER NOT NULL,"
		"item0  INTEGER NOT NULL,"
		"item1  INTEGER NOT NULL,"
		"item2  INTEGER NOT NULL,"
		"item3  INTEGER NOT NULL,"
		"item4  INTEGER NOT NULL,"
		"item5  INTEGER NOT NULL,"
		"unitname  TEXT NOT NULL,"
		"aitem0  INTEGER NOT NULL,"
		"aitem1  INTEGER NOT NULL,"
		"aitem2  INTEGER NOT NULL,"
		"aitem3  INTEGER NOT NULL,"
		"aitem4  INTEGER NOT NULL,"
		"aitem5  INTEGER NOT NULL,"
		"kills  INTEGER NOT NULL,"
		"deaths  INTEGER NOT NULL,"
		"assists  INTEGER NOT NULL,"
		"level  INTEGER NOT NULL,"
		"gold  INTEGER NOT NULL,"
		"lasthits  INTEGER NOT NULL,"
		"denies  INTEGER NOT NULL,"
		"goldpermin  INTEGER NOT NULL,"
		"xppermin  INTEGER NOT NULL,"
		"goldspent  INTEGER NOT NULL,"
		"herodamage  INTEGER NOT NULL,"
		"towerdamage  INTEGER NOT NULL,"
		"herohealing  INTEGER NOT NULL,"
		"leaverstatus  INTEGER NOT NULL,"
		"FOREIGN KEY(matchid) REFERENCES matchdetail (matchid)"
		"); ";

	db.exec(sqlcreatematchdetail_playerinfo);

	static QString sqlindexmatchdetail_playerinfo = "CREATE INDEX IF NOT EXISTS index_matchid ON matchdetail_playerinfo("
		"matchid)";

	db.exec(sqlindexmatchdetail_playerinfo);

	static QString sqlcreatematchdetail_abilitiesupgrade = "CREATE TABLE IF NOT EXISTS matchdetail_abilitiesupgrade("
		"matchid  INTEGER NOT NULL,"
		"slot  INTEGER NOT NULL,"
		"ability  INTEGER NOT NULL,"
		"time  INTEGER NOT NULL,"
		"level  INTEGER NOT NULL,"
		"FOREIGN KEY(matchid) REFERENCES matchdetail (matchid))";

	db.exec(sqlcreatematchdetail_abilitiesupgrade);

	static QString sqlcreateside = "CREATE TABLE IF NOT EXISTS matchdetail_side("
		"matchid  INTEGER NOT NULL,"
		"side  INTEGER NOT NULL,"
		"win  INTEGER NOT NULL,"
		"tower  INTEGER NOT NULL,"
		"barrack  INTEGER NOT NULL,"
		"gpm  INTEGER NOT NULL,"
		"xpm  INTEGER NOT NULL,"
		"herodamage  INTEGER NOT NULL,"
		"towerdamage  INTEGER NOT NULL,"
		"herohealing  INTEGER NOT NULL,"
		"FOREIGN KEY(matchid) REFERENCES matchdetail (matchid)"
		");";

	db.exec(sqlcreateside);

	static QString sqlcreate_matchdetail_items = "CREATE TABLE IF NOT EXISTS matchdetail_items("
		"matchid  INTEGER NOT NULL,"
		"slot  INTEGER NOT NULL,"
		"isunit  INTEGER NOT NULL,"
		"itemslot  INTEGER NOT NULL,"
		"item  INTEGER NOT NULL,"
		"FOREIGN KEY(matchid) REFERENCES matchdetail (matchid)"
		");";

	db.exec(sqlcreate_matchdetail_items);

}

void DataBaseManager::initPlayerInfodbs()
{
	static QString sqlcreatematchdetail_playerinfo = "CREATE TABLE IF NOT EXISTS playerinfo("
		"accountid  INTEGER NOT NULL,"
		"status  INTEGER NOT NULL DEFAULT 0,"
		"PRIMARY KEY(accountid)"
		");";

	db.exec(sqlcreatematchdetail_playerinfo);
	auto error = db.lastError().text();
}

QString DataBaseManager::getTable_HeroesUsedAndRate_Name(const DataConfig &config)
{
	static QString tablenamefmt = "herousedandrate%1";
	return tablenamefmt.arg(config.getFileParams());
}

QString DataBaseManager::getTable_HeroItems_Name(const QString &heroname, const DataConfig &config)
{
	static QString tablenamefmt = "%1%2";
	QString tablename = tablenamefmt.arg(heroname).arg(config.getFileParams());
	tablename.replace(" ", "");
	tablename.replace(".", "");
	return tablename;
}

bool DataBaseManager::loadMatchDetailSide(MatchDetail &matchdetail)
{
	static QString tablename = "matchdetail_side";

	QSqlTableModel model(0, db);
	model.setTable(tablename);
	model.setFilter(QString("matchid=%1").arg(matchdetail.matchid));
	model.select();

	if (model.rowCount() != 2)
		return false;
	else
	{
		for (int i = 0; i < 2; ++i)
		{
			QSqlRecord record;
			int side;

			record = model.record(0);
			side = record.value("side").toInt();
			if (side == 1)
			{
				matchdetail.radianttower = record.value("tower").toInt();
				matchdetail.radiantbarracks = record.value("barrack").toInt();
				matchdetail.radiantgpm = record.value("gpm").toInt();
				matchdetail.radiantxpm = record.value("xpm").toInt();
				matchdetail.radiantherodamage = record.value("herodamage").toInt();
				matchdetail.radianttowerdamage = record.value("towerdamage").toInt();
				matchdetail.radiantherohealing = record.value("herohealing").toInt();
			}
			else
			{
				matchdetail.diretower = record.value("tower").toInt();
				matchdetail.direbarracks = record.value("barrack").toInt();
				matchdetail.diregpm = record.value("gpm").toInt();
				matchdetail.direxpm = record.value("xpm").toInt();
				matchdetail.direherodamage = record.value("herodamage").toInt();
				matchdetail.diretowerdamage = record.value("towerdamage").toInt();
				matchdetail.direherohealing = record.value("herohealing").toInt();
			}
		}
	}

	return true;
}

bool DataBaseManager::loadMatchDetailPickBanList(MatchDetail &matchdetail)
{
	static QString tablename = "matchdetail_pickban";

	QSqlTableModel model(0, db);
	model.setTable(tablename);
	model.setFilter(QString("matchid=%1").arg(matchdetail.matchid));
	model.select();

	if (model.rowCount() == 0)
		return false;
	else
	{
		for (int i = 0; i < model.rowCount(); ++i)
		{
			auto record = model.record(i);
			int order = record.value("bporder").toInt();
			matchdetail.pickbanlist[order].ispick = record.value("ispick").toInt() == 1;
			matchdetail.pickbanlist[order].heroid = record.value("heroid").toInt();
			matchdetail.pickbanlist[order].team = record.value("team").toInt();
		}
	}

	return true;
}

bool DataBaseManager::loadMatchDetailPlayerInfo(MatchDetail &matchdetail)
{
	static QString tablename = "matchdetail_playerinfo";

	QSqlTableModel model(0, db);
	model.setTable(tablename);
	model.setFilter(QString("matchid=%1").arg(matchdetail.matchid));
	model.select();

	if (model.rowCount() == 0)
		return false;
	else
	{
		for (int i = 0; i < model.rowCount(); ++i)
		{
			auto record = model.record(i);
			int slot = record.value("slot").toInt();

			MatchDetail::PlayerDetail *player;
			if (slot < 128)
				player = &matchdetail.radiantplayers[slot];
			else
				player = &matchdetail.direplayers[slot - 128];

			player->accountid = record.value("accountid").toInt();
			player->heroid = record.value("heroid").toInt();
			player->unitname = record.value("unitname").toString();
			player->kills = record.value("kills").toInt();
			player->deaths = record.value("deaths").toInt();
			player->assists = record.value("assists").toInt();
			player->level = record.value("level").toInt();
			player->gold = record.value("gold").toInt();
			player->lasthits = record.value("lasthits").toInt();
			player->denies = record.value("denies").toInt();
			player->goldpermin = record.value("goldpermin").toInt();
			player->xppermin = record.value("xppermin").toInt();
			player->goldspent = record.value("goldspent").toInt();
			player->herodamage = record.value("herodamage").toInt();
			player->towerdamage = record.value("towerdamage").toInt();
			player->herohealing = record.value("herohealing").toInt();
			player->leaverstatus = record.value("leaverstatus").toInt();
		}
	}
	loadMatchDetailPlayerAbilitiesUpgragde(matchdetail);
	loadMatchDetailPlayerItems(matchdetail);
	return true;
}

bool DataBaseManager::loadMatchDetailPlayerAbilitiesUpgragde(MatchDetail &matchdetail)
{
	static QString tablename = "matchdetail_abilitiesupgrade";

	QSqlTableModel model(0, db);
	model.setTable(tablename);
	model.setFilter(QString("matchid=%1").arg(matchdetail.matchid));
	model.select();

	if (model.rowCount() == 0)
		return false;
	else
	{
		for (int i = 0; i < model.rowCount(); ++i)
		{
			auto record = model.record(i);
			int slot = record.value("slot").toInt();

			MatchDetail::PlayerDetail *player;
			if (slot < 128)
				player = &matchdetail.radiantplayers[slot];
			else
				player = &matchdetail.direplayers[slot - 128];

			int level = record.value("level").toInt();
			MatchDetail::PlayerDetail::AbilityUpgradesDetail &ability = player->abilityupgrades[level];
			ability.ability = record.value("ability").toInt();
			ability.time = record.value("time").toInt();
		}
	}

	return true;
}

bool DataBaseManager::loadMatchDetailPlayerItems(MatchDetail &matchdetail)
{
	static QString sqlselect = "select * from matchdetail_items where matchid = %1";
	auto query = db.exec(sqlselect);

	while (query.next())
	{
		int slot = query.value("slot").toInt();
		MatchDetail::PlayerDetail *player;
		if (slot < 128)
			player = &matchdetail.radiantplayers[slot];
		else
			player = &matchdetail.direplayers[slot - 128];

		int itemslot = query.value("itemslot").toInt();
		MatchDetail::PlayerDetail::ItemInfo *iteminfo;
		if (query.value("isunit").toInt() == 1)
			iteminfo = &player->uitem[itemslot];
		else
			iteminfo = &player->item[itemslot];

		iteminfo->id = query.value("item").toInt();
	}
	return true;
}

void DataBaseManager::saveMatchDetailSide(MatchDetail &matchdetail)
{
	static QString tablename = "matchdetail_side";

	QSqlTableModel model(0, db);
	model.setTable(tablename);
	model.setFilter(QString("matchid=%1").arg(matchdetail.matchid));
	model.select();

	if (model.rowCount() != 0)
		return;

	db.transaction();
	static QString sqlinsert = "INSERT INTO matchdetail_side(matchid, side, win, tower, barrack, gpm, xpm, herodamage, towerdamage, herohealing) VALUES(%1, %2, %3, %4, %5, %6, %7, %8, %9, %10);";
	db.exec(sqlinsert.arg(matchdetail.matchid).arg(1).arg(matchdetail.victoryparty == 1 ? 1 : 0).arg(matchdetail.radianttower).arg(matchdetail.radiantbarracks).arg(matchdetail.radiantgpm).arg(matchdetail.radiantxpm).arg(matchdetail.radiantherodamage).arg(matchdetail.radianttowerdamage).arg(matchdetail.radiantherohealing));
	db.exec(sqlinsert.arg(matchdetail.matchid).arg(0).arg(matchdetail.victoryparty == 0 ? 1 : 0).arg(matchdetail.diretower).arg(matchdetail.direbarracks).arg(matchdetail.diregpm).arg(matchdetail.direxpm).arg(matchdetail.direherodamage).arg(matchdetail.diretowerdamage).arg(matchdetail.direherohealing));
	db.commit();
	auto e2 = db.lastError().text();
}

void DataBaseManager::saveMatchDetailPickBanList(MatchDetail& matchdetail)
{
	static QString tablename = "matchdetail_pickban";

	QSqlTableModel model(0, db);
	model.setTable(tablename);
	model.setFilter(QString("matchid=%1").arg(matchdetail.matchid));
	model.select();

	if (model.rowCount() != 0)
		return;

	db.transaction();
	static QString sqlinsert = "INSERT INTO matchdetail_pickban(matchid, ispick, heroid, team, bporder) VALUES(%1, %2, %3, %4, %5);";
	for (int i = 0; i < 20; ++i)
	{
		db.exec(sqlinsert.arg(matchdetail.matchid).arg(matchdetail.pickbanlist[i].ispick ? 1 : 0).arg(matchdetail.pickbanlist[i].heroid).arg(matchdetail.pickbanlist[i].team).arg(i));
	}
	db.commit();
}

void DataBaseManager::saveMatchDetailPlayerInfo(MatchDetail &matchdetail)
{
	static QString tablename = "matchdetail_playerinfo";

	db.transaction();
	static QString sqlinsert = "INSERT INTO matchdetail_playerinfo(matchid, accountid, heroid, slot, unitname"
		", kills, deaths, assists, level, gold, lasthits, denies, goldpermin, xppermin, goldspent, herodamage, towerdamage, herohealing, leaverstatus) "
		"VALUES(%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14, %15, %16, %17, %18, %19);";
	for (int i = 0; i < 5; ++i)
	{
		MatchDetail::PlayerDetail &player = matchdetail.radiantplayers[i];
		db.exec(sqlinsert.arg(matchdetail.matchid).arg(player.accountid).arg(player.heroid).arg(i)
			.arg("'" + player.unitname + "'").arg(player.kills).arg(player.deaths).arg(player.assists)
			.arg(player.level).arg(player.gold).arg(player.lasthits).arg(player.denies).arg(player.goldpermin).arg(player.xppermin).arg(player.goldspent).arg(player.herodamage).arg(player.towerdamage).arg(player.herohealing).arg(player.leaverstatus));
	}
	for (int i = 0; i < 5; ++i)
	{
		MatchDetail::PlayerDetail &player = matchdetail.direplayers[i];
		db.exec(sqlinsert.arg(matchdetail.matchid).arg(player.accountid).arg(player.heroid).arg(i + 128)
			.arg("'" + player.unitname + "'").arg(player.kills).arg(player.deaths).arg(player.assists)
			.arg(player.level).arg(player.gold).arg(player.lasthits).arg(player.denies).arg(player.goldpermin).arg(player.xppermin).arg(player.goldspent).arg(player.herodamage).arg(player.towerdamage).arg(player.herohealing).arg(player.leaverstatus));
	}

	db.commit();
	QSqlError error = db.lastError();
	QString errortext = error.text();

	saveMatchDetailPlayerAbilitiesUpgragde(matchdetail);
	saveMatchDetailPlayerItems(matchdetail);
}

void DataBaseManager::saveMatchDetailPlayerAbilitiesUpgragde(MatchDetail &matchdetail)
{
	static QString tablename = "matchdetail_abilitiesupgrade";

	db.transaction();
	static QString sqlinsert = "INSERT INTO matchdetail_abilitiesupgrade(matchid, slot, ability, time, level) "
		"VALUES(%1, %2, %3, %4, %5);";
	for (int i = 0; i < 5; ++i)
	{
		MatchDetail::PlayerDetail &player = matchdetail.radiantplayers[i];
		for (int j = 0; j < 25; ++j)
		{
			MatchDetail::PlayerDetail::AbilityUpgradesDetail &abilities = player.abilityupgrades[j];
			if (abilities.ability == 0)
				break;
			db.exec(sqlinsert.arg(matchdetail.matchid).arg(i).arg(abilities.ability).arg(abilities.time).arg(j));
		}
	}
	for (int i = 0; i < 5; ++i)
	{
		MatchDetail::PlayerDetail &player = matchdetail.direplayers[i];
		for (int j = 0; j < 25; ++j)
		{
			MatchDetail::PlayerDetail::AbilityUpgradesDetail &abilities = player.abilityupgrades[j];
			if (abilities.ability == 0)
				break;
			db.exec(sqlinsert.arg(matchdetail.matchid).arg(i + 128).arg(abilities.ability).arg(abilities.time).arg(j));
		}
	}

	db.commit();
}

void DataBaseManager::saveMatchDetailPlayerItems(MatchDetail &matchdetail)
{
	static QString sqlinsert = "INSERT INTO matchdetail_items(matchid, slot, isunit, itemslot, item) "
		"VALUES(%1, %2, %3, %4, %5);";
	db.transaction();

	for (int i = 0; i < 5; ++i)
	{
		MatchDetail::PlayerDetail &player = matchdetail.radiantplayers[i];
		for (int j = 0; j < 6; ++j)
		{
			MatchDetail::PlayerDetail::ItemInfo &item = player.item[j];
			db.exec(sqlinsert.arg(matchdetail.matchid).arg(i).arg(0).arg(j).arg(item.id));
		}
		if (!player.unitname.isEmpty())
		{
			for (int j = 0; j < 6; ++j)
			{
				MatchDetail::PlayerDetail::ItemInfo &item = player.uitem[j];
				db.exec(sqlinsert.arg(matchdetail.matchid).arg(i).arg(1).arg(j).arg(item.id));
			}
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		MatchDetail::PlayerDetail &player = matchdetail.direplayers[i];
		for (int j = 0; j < 6; ++j)
		{
			MatchDetail::PlayerDetail::ItemInfo &item = player.item[j];
			db.exec(sqlinsert.arg(matchdetail.matchid).arg(i).arg(0).arg(j).arg(item.id));
		}
		if (!player.unitname.isEmpty())
		{
			for (int j = 0; j < 6; ++j)
			{
				MatchDetail::PlayerDetail::ItemInfo &item = player.uitem[j];
				db.exec(sqlinsert.arg(matchdetail.matchid).arg(i).arg(1).arg(j).arg(item.id));
			}
		}
	}

	db.commit();
}
