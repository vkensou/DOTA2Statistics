#ifndef HEROITEMS_H
#define HEROITEMS_H

#include <QString>
#include <QHash>

class DataManager;

//hero winning rate with item
class HeroItems
{
public:
    const QString & getName() const{return m_name;}
    HeroItems(const QString &name);

    void download();
    void load(bool force_download = false);
    void save();

    void calcX2(int heroused, float herorate);

    double getX2(int heroused, double herorate, const QString &name);
    static double getX2(int heroused, double herorate, int itemused, double itemrate);

    struct ItemRateAndUsed
    {
        ItemRateAndUsed(const QString &name, int used, double rate)
            :name(name), used(used), rate(rate)
        {
        }

        QString name;
        int used;
        double rate;
        double x2;

        bool operator < (const ItemRateAndUsed &a) const
        {
            return a.name < name;
        }
    };
    QHash<QString, ItemRateAndUsed> list;
private:
    void parseWebPageData(const QString &data);
    void addItem(const QString &name, int used, double rate);

    QString m_name;

    QString getHeroItemsFilename();
    QString getHeroItemsX2Filename();

    friend DataManager;
};

#endif // HEROITEMS_H
