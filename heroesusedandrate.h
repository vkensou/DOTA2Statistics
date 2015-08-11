#ifndef HEROESRATE_H
#define HEROESRATE_H

#include <QString>
#include <QUrl>
#include <QHash>
#include <functional>

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
{
public:
	HeroesUsedAndRate();
	~HeroesUsedAndRate();
	HeroesUsedAndRate(const HeroesUsedAndRate&) = delete;

    void download();
    void load(bool force_download = false);
    void save();

    float getRate(const QString &chinese_name);
    int getUsed(const QString &chinese_name);

private:
	QHash<QString, HeroRateAndUsed *> m_list;
	std::function<void(const QString &, int, double)> m_addHero_callback;
	std::function<void(std::function<void(const HeroRateAndUsed *)>)> m_enumList;

    QString getHeroesUsedAndRateFilename();
    void addHero(const QString &name, int used, double rate);
	HeroRateAndUsed * getHero(const QString &chinese_name);
};

#endif // HEROESRATE_H
