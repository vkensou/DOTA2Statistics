#ifndef HEROITEMS_H
#define HEROITEMS_H

#include <QString>
#include <QHash>

//hero winning rate with item
class HeroItems
{
public:
    const QString & getName(){return m_name;}
    HeroItems(const QString &name);

    void download();
    void load();
    void save();

    void calcX2(int heroused, float herorate);

    double getX2(int heroused, double herorate, const QString &name);
    static double getX2(int heroused, double herorate, int itemused, double itemrate);

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
private:
    void parseWebPageData(const QString &data);
    void addItem(const QString &name, float rate, int used);

    QString m_name;

    QString getHeroItemsFilename();
    QString getHeroItemsX2Filename();

};

#endif // HEROITEMS_H
