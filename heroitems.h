#ifndef HEROITEMS_H
#define HEROITEMS_H

#include <QString>
#include <QHash>
#include <functional>

class ItemRateAndUsed
{
public:
	ItemRateAndUsed(const QString &name, int used, double rate)
		:name(name), used(used), rate(rate)
	{
	}

	ItemRateAndUsed(const QString &name, int used, double rate, double x2)
		:name(name), used(used), rate(rate), x2(x2)
	{
	}

	ItemRateAndUsed(const ItemRateAndUsed &) = delete;

	QString name;
	int used;
	double rate;
	double x2{ 0 };

	bool operator < (const ItemRateAndUsed &a) const
	{
		return a.name < name;
	}
};

//hero winning rate with item
class HeroItems
{
public:
    const QString & getName() const{return m_name;}
    HeroItems(const QString &name);
	~HeroItems();
	HeroItems(const HeroItems &) = delete;

    void download();
    void load(bool force_download = false);
    void save();
    void saveasxml();


    int getItemsCount() const;
    void for_each_items(std::function<void(ItemRateAndUsed *)> func);
    void for_each_items(std::function<void(const ItemRateAndUsed *)> func) const;

private:
    void calcX2(int heroused, float herorate);
    double getX2(int heroused, double herorate, int itemused, double itemrate);
    void addItem(const QString &name, int used, double rate, double x2);
    QString getHeroItemsFilename();
    QString getHeroItemsX2Filename();

    QString m_name;
    QString m_chinese_name;

	QHash<QString, ItemRateAndUsed *> m_list;
	std::function<void(const QString &, int, double, double)> m_addItem_callback;
	std::function<void(std::function<void(const ItemRateAndUsed *)>)> m_enumList;
};

#endif // HEROITEMS_H
