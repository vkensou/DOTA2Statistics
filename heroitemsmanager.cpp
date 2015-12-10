#include "heroitemsmanager.h"
#include "dataconfig.h"
#include "utility.h"


HeroItems &HeroItemsManager::getHeroItems(const QString &name, const DataConfig &config)
{
    static QString keyfmt = "%1%2";
    QString key = keyfmt.arg(name).arg(config.getFileParams());

	std::function<HeroItems*()> func = [&name]()
	{
		return new HeroItems(name);
	};

	return getDatum(key, func);
}
