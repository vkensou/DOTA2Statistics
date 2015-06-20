#ifndef HEROESRATE_H
#define HEROESRATE_H

#include <QString>
#include <QHash>

class HeroesRateAndUsed
{
public:
    void download(const QString &time = "v684", const QString &server = "cn", const QString &ladder = "y", const QString &skill = "vh");
    void load(const QString &filename);
    void save(const QString &filename);

    float getRate(const QString &name);
    int getUsed(const QString &name);
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
};

#endif // HEROESRATE_H
