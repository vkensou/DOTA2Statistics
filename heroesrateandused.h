#ifndef HEROESRATE_H
#define HEROESRATE_H

#include <QString>
#include <QHash>

class HeroesRateAndUsed
{
public:
    void download();
    void load();
    void save();

    float getRate(const QString &chinese_name);
    int getUsed(const QString &chinese_name);
private:
    void parseWebPageData(const QString &data);
    struct HeroRateAndUsed
    {
        HeroRateAndUsed(const QString &name, float rate, int used)
            :name(name), rate(rate), used(used)
        {
        }

        QString name;
        float rate;
        int used;

        bool operator < (const HeroRateAndUsed &a) const
        {
            return a.name < name;
        }
    };
    QHash<QString, HeroRateAndUsed> list;

    QString getHeroesRateAndUsedFilename();
};

#endif // HEROESRATE_H
