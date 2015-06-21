#ifndef HEROESRATE_H
#define HEROESRATE_H

#include <QString>
#include <QUrl>
#include <QHash>

class DataManager;

class HeroesUsedAndRate
{
public:
    void download();
    void load(bool force_download = false);
    void save();

    float getRate(const QString &chinese_name);
    int getUsed(const QString &chinese_name);

private:
    void parseWebPageData(const QString &data);
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

    QUrl getHeroesUsedAndRateUrl();
    QString getHeroesUsedAndRateFilename();
    void addHero(const QString &name, int used, double rate);

    friend DataManager;
};

#endif // HEROESRATE_H
