#ifndef HEROESRATE_H
#define HEROESRATE_H

#include <QString>
#include <QUrl>
#include <QHash>

class DataBaseManager;
class WebDataDownloader;

class HeroesUsedAndRate
{
public:
    void download();
    void load(bool force_download = false);
    void save();

    float getRate(const QString &chinese_name);
    int getUsed(const QString &chinese_name);

private:
    struct HeroRateAndUsed
    {
        HeroRateAndUsed(const QString &name, int used, double rate)
            :name(name), used(used), rate(rate)
        {
        }

        QString name;
        int used;
        double rate;

        bool operator < (const HeroRateAndUsed &a) const
        {
            return a.name < name;
        }
    };
    QHash<QString, HeroRateAndUsed> list;

    QString getHeroesUsedAndRateFilename();
	void clear();
    void addHero(const QString &name, int used, double rate);

    friend DataBaseManager;
	friend WebDataDownloader;
};

#endif // HEROESRATE_H
