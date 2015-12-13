#ifndef HEROESRATE_H
#define HEROESRATE_H

#include <QString>
#include <QUrl>
#include <QHash>
#include <functional>
#include "hashdataneedcache.h"

class DataBaseManager;
class WebDataDownloader;

class HeroRateAndUsed
{
public:
	HeroRateAndUsed(const QString &name, int used, double rate);
	HeroRateAndUsed(const HeroRateAndUsed&) = delete;

	QString name;
	int used;
	double rate;

	bool operator < (const HeroRateAndUsed &a) const
	{
		return a.name < name;
	}
};

class HeroesUsedAndRate
	:public HashDataNeedCache<HeroRateAndUsed>
{
public:
	HeroesUsedAndRate();
	HeroesUsedAndRate(const HeroesUsedAndRate&) = delete;

    float getRate(const QString &chinese_name);
    int getUsed(const QString &chinese_name);

private:
	std::function<void(const QString &, int, double)> m_addHero_callback;
	std::function<void(std::function<void(const HeroRateAndUsed *)>)> m_enumList;

    QString getHeroesUsedAndRateFilename();
    void addHero(const QString &name, int used, double rate);
	HeroRateAndUsed * getHero(const QString &chinese_name);

	virtual bool loadFromDataBase() override;
	virtual void download() override;
	virtual void save() override;

};

#endif // HEROESRATE_H
