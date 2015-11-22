#include "databasemanager.h"
#include "dataconfig.h"
#include "heroesusedandrate.h"
#include "heroitems.h"
#include <functional>
#include <algorithm>

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

bool DataBaseManager::loadHeroesUsedAndRate(std::function<void(const QString &, int, double)> callback, const DataConfig &config)
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

bool DataBaseManager::loadHeroItems(const QString &heroname, std::function<void(const QString &, int, double, double)> callback, const DataConfig &config)
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
