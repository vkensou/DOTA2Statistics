#include "abilitieslist.h"

#include <QDomDocument>
#include <QFile>

bool AbilitiesList::load()
{
	QFile file("abilities.xml");

	if (file.open(QIODevice::ReadOnly))
	{
		QDomDocument doc;
		doc.setContent(&file);

		auto root = doc.documentElement();

		if (!root.isNull())
		{
			for (auto node = root.firstChildElement("ability"); !node.isNull(); node = node.nextSiblingElement("ability"))
			{
				int id = node.firstChildElement("id").text().toInt();
				int index = 0;
				auto indexnode = node.firstChildElement("index");
				if (!indexnode.isNull())
					index = indexnode.text().toInt();

				m_abilities.insert(std::pair<int, int>(id, index));
			}
		}

		return true;
	}
	return false;
}

char AbilitiesList::getIndexByID(int id)
{
	if (id == 5002)
		return '+';

	auto iter = m_abilities.find(id);
	if (iter != m_abilities.end())
	{
		int index = iter->second;
		switch (index)
		{
		case 1:
			return '1';
		case 2:
			return '2';
		case 3:
			return '3';
		case 4:
			return '4';
		}
	}
	
	return '?';
}
