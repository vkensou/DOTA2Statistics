#ifndef HEROITEMS_H
#define HEROITEMS_H

#include <QString>
#include <QHash>

//hero winning rate with item
class HeroItems
{
public:
    HeroItems(const QString &name);

    void download();
    void load();
    void save();

    void calcX2(int heroused, float herorate);

    float getX2(int heroused, float herorate, const QString &name);

private:
    void parseWebPageData(const QString &data);

    QString m_name;
    struct ItemRateAndUsed
    {
        ItemRateAndUsed(const QString &name, float rate, int used)
            :name(name), rate(rate), used(used)
        {
        }

        QString name;
        float rate;
        int used;
        float x2;

        bool operator < (const ItemRateAndUsed &a) const
        {
            return a.name < name;
        }
    };
    QHash<QString, ItemRateAndUsed> list;
};

#endif // HEROITEMS_H
