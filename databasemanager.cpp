#include "databasemanager.h"
#include "dataconfig.h"
#include "heroesusedandrate.h"
#include "heroitems.h"
#include <functional>
#include <algorithm>
#include "matchdetail.h"

DataBaseManager::DataBaseManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dota2satistics.db");

}

bool DataBaseManager::opendb()
{
    return db.open();
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
		matchdetail.radianttower = record.value("radianttower").toInt();
		matchdetail.diretower = record.value("diretower").toInt();
		matchdetail.radiantbarracks = record.value("radiantbarracks").toInt();
		matchdetail.direbarracks = record.value("direbarracks").toInt();
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

	if (matchdetail.lobbytype == 2 && !loadMatchDetailPickBanList(matchdetail))
		return false;

	return loadMatchDetailPlayerInfo(matchdetail);
}

void DataBaseManager::saveMatchDetail(MatchDetail &matchdetail)
{
	static QString tablename = "matchdetail";

	static QString sqlcreate = "CREATE TABLE IF NOT EXISTS matchdetail(matchid  INTEGER NOT NULL,"
		"victoryparty  INTEGER NOT NULL,"
		"duration  INTEGER NOT NULL,"
		"seqnum  INTEGER NOT NULL,"
		"radianttower  INTEGER NOT NULL,"
		"diretower  INTEGER NOT NULL,"
		"radiantbarracks  INTEGER NOT NULL,"
		"direbarracks  INTEGER NOT NULL,"
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

	db.exec(sqlcreate.arg(tablename));

	QSqlTableModel model(0, db);
	model.setTable(tablename);
	model.setFilter(QString("matchid=%1").arg(matchdetail.matchid));
	model.select();

	if (model.rowCount() != 0)
		return;

	static QString sqlinsert = "INSERT INTO matchdetail(matchid, victoryparty, duration, seqnum, radianttower, diretower, radiantbarracks, direbarracks, cluster, firstbloodtime, lobbytype, humanplayer, leagueid, positivevotes, negativevotes, gamemode, engine, starttime) "
		"VALUES(%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14, %15, %16, %17, %18);";

	db.exec(sqlinsert.arg(matchdetail.matchid).arg(matchdetail.victoryparty).arg(matchdetail.duration).arg(matchdetail.matchseqnum).arg(matchdetail.radianttower).arg(matchdetail.diretower)
		.arg(matchdetail.radiantbarracks).arg(matchdetail.direbarracks).arg(matchdetail.cluster).arg(matchdetail.firstbloodtime).arg(matchdetail.lobbytype).arg(matchdetail.humanplayer)
		.arg(matchdetail.leagueid).arg(matchdetail.positivevotes).arg(matchdetail.negativevotes).arg(matchdetail.gamemode).arg(matchdetail.engine).arg(matchdetail.starttime));

	if (matchdetail.gamemode == 2)
		saveMatchDetailPickBanList(matchdetail);

	saveMatchDetailPlayerInfo(matchdetail);
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
			player->item0 = record.value("item0").toInt();
			player->item1 = record.value("item1").toInt();
			player->item2 = record.value("item2").toInt();
			player->item3 = record.value("item3").toInt();
			player->item4 = record.value("item4").toInt();
			player->item5 = record.value("item5").toInt();
			player->unitname = record.value("unitname").toString();
			player->aitem0 = record.value("aitem0").toInt();
			player->aitem1 = record.value("aitem1").toInt();
			player->aitem2 = record.value("aitem2").toInt();
			player->aitem3 = record.value("aitem3").toInt();
			player->aitem4 = record.value("aitem4").toInt();
			player->aitem5 = record.value("aitem5").toInt();
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

	return loadMatchDetailPlayerAbilitiesUpgragde(matchdetail);
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

void DataBaseManager::saveMatchDetailPickBanList(MatchDetail& matchdetail)
{
	static QString tablename = "matchdetail_pickban";

	static QString sqlcreate = "CREATE TABLE IF NOT EXISTS matchdetail_pickban("
		"matchid  INTEGER NOT NULL,"
		"ispick  INTEGER NOT NULL,"
		"heroid  INTEGER NOT NULL,"
		"team  INTEGER NOT NULL,"
		"bporder  INTEGER NOT NULL,"
		"FOREIGN KEY(matchid) REFERENCES matchdetail (matchid),"
		");";

	db.exec(sqlcreate);

	static QString sqlindex = "CREATE INDEX IF NOT EXISTS index_matchid ON matchdetail_pickban("
		"matchid)";

	db.exec(sqlindex);

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

	static QString sqlcreate = "CREATE TABLE IF NOT EXISTS matchdetail_playerinfo("
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

	db.exec(sqlcreate);

	static QString sqlindex = "CREATE INDEX IF NOT EXISTS index_matchid ON matchdetail_playerinfo("
		"matchid)";

	db.exec(sqlindex);

	db.transaction();
	static QString sqlinsert = "INSERT INTO matchdetail_playerinfo(matchid, accountid, heroid, slot, item0, item1, item2, item3, item4, item5, unitname, aitem0, aitem1, aitem2, aitem3, aitem4, aitem5"
		", kills, deaths, assists, level, gold, lasthits, denies, goldpermin, xppermin, goldspent, herodamage, towerdamage, herohealing, leaverstatus) "
		"VALUES(%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14, %15, %16, %17, %18, %19, %20, %21, %22, %23, %24, %25, %26, %27, %28, %29, %30, %31);";
	for (int i = 0; i < 5; ++i)
	{
		MatchDetail::PlayerDetail &player = matchdetail.radiantplayers[i];
		db.exec(sqlinsert.arg(matchdetail.matchid).arg(player.accountid).arg(player.heroid).arg(i).arg(player.item0).arg(player.item1).arg(player.item2).arg(player.item3).arg(player.item4).arg(player.item5)
			.arg("'" + player.unitname + "'").arg(player.aitem0).arg(player.aitem1).arg(player.aitem2).arg(player.aitem3).arg(player.aitem4).arg(player.aitem5).arg(player.kills).arg(player.deaths).arg(player.assists)
			.arg(player.level).arg(player.gold).arg(player.lasthits).arg(player.denies).arg(player.goldpermin).arg(player.xppermin).arg(player.goldspent).arg(player.herodamage).arg(player.towerdamage).arg(player.herohealing).arg(player.leaverstatus));
	}
	for (int i = 0; i < 5; ++i)
	{
		MatchDetail::PlayerDetail &player = matchdetail.direplayers[i];
		db.exec(sqlinsert.arg(matchdetail.matchid).arg(player.accountid).arg(player.heroid).arg(i + 128).arg(player.item0).arg(player.item1).arg(player.item2).arg(player.item3).arg(player.item4).arg(player.item5)
			.arg("'" + player.unitname + "'").arg(player.aitem0).arg(player.aitem1).arg(player.aitem2).arg(player.aitem3).arg(player.aitem4).arg(player.aitem5).arg(player.kills).arg(player.deaths).arg(player.assists)
			.arg(player.level).arg(player.gold).arg(player.lasthits).arg(player.denies).arg(player.goldpermin).arg(player.xppermin).arg(player.goldspent).arg(player.herodamage).arg(player.towerdamage).arg(player.herohealing).arg(player.leaverstatus));
	}

	db.commit();
	QSqlError error = db.lastError();
	QString errortext = error.text();

	saveMatchDetailPlayerAbilitiesUpgragde(matchdetail);
}

void DataBaseManager::saveMatchDetailPlayerAbilitiesUpgragde(MatchDetail &matchdetail)
{
	static QString tablename = "matchdetail_abilitiesupgrade";

	static QString sqlcreate = "CREATE TABLE IF NOT EXISTS matchdetail_abilitiesupgrade("
		"matchid  INTEGER NOT NULL,"
		"slot  INTEGER NOT NULL,"
		"ability  INTEGER NOT NULL,"
		"time  INTEGER NOT NULL,"
		"level  INTEGER NOT NULL,"
		"FOREIGN KEY(matchid) REFERENCES matchdetail (matchid))";

	db.exec(sqlcreate);
	QSqlError error = db.lastError();
	QString errortext = error.text();

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
	error = db.lastError();
	errortext = error.text();

}
