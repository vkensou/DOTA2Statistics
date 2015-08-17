#ifndef DATACONFIG_H
#define DATACONFIG_H

#include <QString>

class DataConfig
{
public:
    QString getFileParams() const;
    void save(const QString &filename);
    void load(const QString &filename);

    static DataConfig & getCurrentConfig();
    static void setCurrentConfig(DataConfig &config);
    //static QString getUrlParamsCurrent();
    static QString getFileParamsCurrent();
    static void saveCurrent(const QString &filename);
    static void loadCurrent(const QString &filename);

	unsigned char webdatasource;
    unsigned char matchtype;
	unsigned char skill;
	unsigned char time;
	unsigned char server;

private:
	const char * getMatchTypeStr(unsigned char matchtype) const;
	const char * getSkillStr(unsigned char skill) const;
	const char * getTimeStr(unsigned char time) const;
	const char * getServerStr(unsigned char server) const;
};

#endif // DATACONFIG_H
