#ifndef HEROLIST_H
#define HEROLIST_H

#include <vector>
#include <QString>
#include <QStringList>
#include "Singleton.h"

class HeroList
        :public Singleton<HeroList>
{
public:
	class Hero
	{
	public:
		Hero(int id, const QString &name, const QString &chinese_name)
			:id(id), name(name), chinese_name(chinese_name)
		{
		}

		int id;
		QString name;
		QString chinese_name;
		QStringList aliases;

		bool operator < (const Hero &a) const
		{
			return a.id < id;
		}
	};

public:
    void load();
    QString getNameByChineseName(const QString &chinesname);
	QString getNameByAlias(const QString &alias);
	QString getChineseNameByName(const QString &name);
	const std::vector<Hero> & getHeroList();

private:
	Hero & addHero(int id, const QString &name, const QString &chinese_name);

private:

    std::vector<Hero> m_list;
};

#endif // HEROLIST_H
