#include "herolist.h"
#include <QString>
#include <QTextStream>
#include <QDomDocument>
#include <QFile>
#include <functional>
#include <algorithm>
#include <QRegExp>

const QString herolistpath = "heroes.xml";

void HeroList::addHero(int id, const QString &name, const QString &chinese_name)
{
    m_list.push_back({id, name, chinese_name});
}

void HeroList::load()
{
    m_list.clear();

    QFile file(herolistpath);

    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument doc;
        doc.setContent(&file);

        auto root = doc.documentElement();

        if(!root.isNull())
        {
            for(auto heronode = root.firstChildElement("hero"); !heronode.isNull(); heronode = heronode.nextSiblingElement())
            {
                int id = heronode.attribute("id").toInt();
                QString name = heronode.attribute("name");
                QString chinese_name = heronode.attribute("chinese_name");
                addHero(id, name, chinese_name);
            }
        }
    }
}

QString HeroList::getNameByChineseName(const QString &chinesname)
{
    auto i = std::find_if(m_list.begin(), m_list.end(), [&chinesname](const Hero &hero){
        return hero.chinese_name == chinesname;
    });
    if(i != m_list.end())
        return (*i).name;
    else
        return "";
}

QString HeroList::getChineseNameByName(const QString &name)
{
    auto i = std::find_if(m_list.begin(), m_list.end(), [&name](const Hero &hero){
        return hero.name == name;
    });
    if(i != m_list.end())
        return (*i).chinese_name;
    else
        return "";}
