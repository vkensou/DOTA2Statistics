#ifndef DATACONFIG_H
#define DATACONFIG_H

#include <QString>

class DataConfig
{
public:
    QString getFileParams() const;
    void save(const QString &filename);
    void load(const QString &filename);
	void reset();

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
};

#endif // DATACONFIG_H
