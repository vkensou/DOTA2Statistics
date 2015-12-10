#ifndef HEROITEMS_H
#define HEROITEMS_H

#include <QString>
#include <QHash>
#include <functional>
#include "hashdataneedcache.h"
#include "utility.h"

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
	:public HashDataNeedCache<ItemRateAndUsed>
{
public:
    const QString & getName() const{return m_name;}
    HeroItems(const QString &name);
	HeroItems(const HeroItems &) = delete;

    void saveasxml();

    int getItemsCount() const;
    void for_each_items(std::function<void(ItemRateAndUsed *)> &func);
    void for_each_items(std::function<void(const ItemRateAndUsed *)> &func) const;

private:
    void calcX2(int heroused, float herorate);
    double getX2(int heroused, double herorate, int itemused, double itemrate);
    void addItem(const QString &name, int used, double rate, double x2);
    QString getHeroItemsFilename();
    QString getHeroItemsX2Filename();

	virtual bool loadFromDataBase() override;
	virtual void download() override;
	virtual void save() override;
	virtual void handleData(bool force_download = false) override;

    QString m_name;
    QString m_chinese_name;

	std::function<void(const QString &, int, double, double)> m_addItem_callback;
	std::function<void(std::function<void(const ItemRateAndUsed *)>)> m_enumList;

};

#endif // HEROITEMS_H
