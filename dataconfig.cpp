#include "dataconfig.h"
#include <QSettings>

static DataConfig curconfig;

QString DataConfig::getUrlParams()
{
    QString p;
    p.sprintf("&ladder=%s&skill=%s&time=%s&server=%s", getMatchTypeStr(matchtype), getSkillStr(skill), getTimeStr(time), getServerStr(server));
    return p;
}

QString DataConfig::getFileParams()
{
    QString p;
    p.sprintf("_%s_%s_%s_%s", getMatchTypeStr(matchtype), getSkillStr(skill), getTimeStr(time), getServerStr(server));
    return p;
}

void DataConfig::save(const QString &filename)
{
    QSettings ini(filename, QSettings::IniFormat);
    ini.setValue("data/matchtype", (int)matchtype);
    ini.setValue("data/skill", (int)skill);
    ini.setValue("data/time", (int)time);
    ini.setValue("data/server", (int)server);
}

void DataConfig::load(const QString &filename)
{
    QSettings ini(filename, QSettings::IniFormat);
    matchtype = (MatchType)ini.value("data/matchtype", 0).toInt();
    skill = (Skill)ini.value("data/skill", 0).toInt();
    time = (Time)ini.value("data/time", 0).toInt();
    server = (Server)ini.value("data/server", 0).toInt();
}

DataConfig & DataConfig::getCurrentConfig()
{
    return curconfig;
}

void DataConfig::setCurrentConfig(DataConfig &config)
{
    curconfig = config;
}

QString DataConfig::getUrlParamsCurrent()
{
    return curconfig.getUrlParams();
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

const char * DataConfig::getMatchTypeStr(DataConfig::MatchType matchtype)
{
    static const char *str[] =
    {
        "all",
        "y",
        "n",
        "solo"
    };
    return str[(int)matchtype];
}

const char * DataConfig::getSkillStr(DataConfig::Skill skill)
{
    static const char *str[] =
    {
        "all",
        "n",
        "h",
        "vh",
        "pro"
    };
    return str[(int)skill];
}

const char * DataConfig::getTimeStr(DataConfig::Time time)
{
    static const char *str[] =
    {
        "all",
        "month",
        "week",
        "v684",
        "v683",
        "v682"
    };
    return str[(int)time];
}

const char * DataConfig::getServerStr(DataConfig::Server server)
{
    static const char *str[] =
    {
        "all",
        "cn",
        "world"
    };
    return str[(int)server];
}
