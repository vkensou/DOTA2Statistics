#include "dataconfig.h"
#include <QSettings>
#include "webdatasourcemanager.h"
#include "iwebdatasource.h"

static DataConfig curconfig;

QString DataConfig::getFileParams() const
{
	return WebDataSourceManager::getInstance().getWebDataSourceCurrent()->getFileParams(*this);
}

void DataConfig::save(const QString &filename)
{
    QSettings ini(filename, QSettings::IniFormat);
	ini.setValue("data/webdatasource", webdatasource);
	ini.setValue("data/matchtype", matchtype);
    ini.setValue("data/skill", skill);
    ini.setValue("data/time", time);
    ini.setValue("data/server", server);
}

void DataConfig::load(const QString &filename)
{
    QSettings ini(filename, QSettings::IniFormat);
	webdatasource = ini.value("data/webdatasource", 0).toInt();
    matchtype = ini.value("data/matchtype", 0).toInt();
    skill = ini.value("data/skill", 0).toInt();
    time = ini.value("data/time", 0).toInt();
    server = ini.value("data/server", 0).toInt();
}

void DataConfig::reset()
{
	matchtype = 0;
	skill = 0;
	time = 0;
	server = 0;
}

DataConfig & DataConfig::getCurrentConfig()
{
    return curconfig;
}

void DataConfig::setCurrentConfig(DataConfig &config)
{
    curconfig = config;
}

QString DataConfig::getFileParamsCurrent()
{
	return curconfig.getFileParams();
}

void DataConfig::saveCurrent(const QString &filename)
{
    curconfig.save(filename);
}

void DataConfig::loadCurrent(const QString &filename)
{
    curconfig.load(filename);
}
