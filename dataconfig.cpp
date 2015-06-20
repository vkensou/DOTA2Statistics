#include "dataconfig.h"

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

DataConfig & DataConfig::getCurrentConfig()
{
    return curconfig;
}

void DataConfig::setCurrentConfig(DataConfig &config)
{
    curconfig = config;
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
        "pro",
        "vh",
        "h",
        "n"
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
