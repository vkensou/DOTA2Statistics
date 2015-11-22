#include "herolist.h"
#include <QString>
#include <QTextStream>
#include <QDomDocument>
#include <QFile>
#include <functional>
#include <algorithm>
#include <QRegExp>

const QString herolistpath = "heroes.xml";

HeroList::Hero & HeroList::addHero(int id, const QString &name, const QString &chinese_name)
{
    m_list.push_back({id, name, chinese_name});
	return m_list.back();
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
            for(auto heronode = root.firstChildElement("hero"); !heronode.isNull(); heronode = heronode.nextSiblingElement("hero"))
            {
                int id = heronode.attribute("id").toInt();
                QString name = heronode.attribute("name");
                QString chinese_name = heronode.attribute("chinese_name");
                auto &hero = addHero(id, name, chinese_name);
				hero.aliases.push_back(name.toLower());
				hero.aliases.push_back(chinese_name);
				for (auto aliasnode = heronode.firstChildElement("alias"); !aliasnode.isNull(); aliasnode = aliasnode.nextSiblingElement("alias"))
				{
					QString alias = aliasnode.text().toLower();
					if (!alias.isEmpty())
						hero.aliases.push_back(alias);
				}
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

QString HeroList::getNameByAlias(const QString &alias)
{
	auto i = std::find_if(m_list.begin(), m_list.end(), [&alias](const Hero &hero){
		return hero.aliases.contains(alias);
	});
	if (i != m_list.end())
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
        return "";
}

const std::vector<HeroList::Hero> & HeroList::getHeroList()
{
	return m_list;
}
