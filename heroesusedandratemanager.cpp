#include "heroesusedandratemanager.h"
#include "utility.h"

HeroesUsedAndRate &HeroesUsedAndRateManager::getHeroesUsedAndRate(bool force_download, const DataConfig &config)
{
    static QString keyfmt = "heroesusedandrate%1";
    QString key = keyfmt.arg(config.getFileParams());

	std::function<HeroesUsedAndRate*()> func = []()
	{
		return new HeroesUsedAndRate;
	};

	auto &datum = getDatum(key, func);
	datum.load(force_download);
	return datum;
}
