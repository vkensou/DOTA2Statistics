#ifndef HEROLIST_H
#define HEROLIST_H

#include <vector>
#include <QString>
#include "Singleton.h"

class HeroList
        :public Singleton<HeroList>
{
public:
    void addHero(int id, const QString &name, const QString &chinese_name);
    void load();
    QString getNameByChineseName(const QString &chinesname);
    QString getChineseNameByName(const QString &name);

private:
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

        bool operator < (const Hero &a) const
        {
            return a.id < id;
        }
    };

    std::vector<Hero> m_list;
};

#endif // HEROLIST_H
