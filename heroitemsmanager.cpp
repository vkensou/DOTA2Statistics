#include "heroitemsmanager.h"
#include "dataconfig.h"
#include "utility.h"
HeroItemsManager::HeroItemsManager()
{
}

HeroItemsManager::~HeroItemsManager()
{
	pointerContainerDeleteAndClear(m_list);
}

HeroItems &HeroItemsManager::getHeroItems(const QString &name, const DataConfig &config)
{
    static QString keyfmt = "%1%2";
    QString key = keyfmt.arg(name).arg(config.getFileParams());

    auto i = m_list.find(key);
	if (i == m_list.end())
    {
		i = m_list.insert(key, new HeroItems(name));
    }
	HeroItems *data = i.value();
	return *data;
}
