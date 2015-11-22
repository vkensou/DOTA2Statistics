#include "webdatadownloader.h"
#include "statusbarsetter.h"
#include "Utility.h"
#include <QDomDocument>
#include "heroesusedandrate.h"
#include "heroitems.h"
#include "dataconfig.h"
#include "webdatasourcemanager.h"
#include "iwebdatasource.h"

WebDataDownloader::WebDataDownloader()
{
}


WebDataDownloader::~WebDataDownloader()
{
}

bool WebDataDownloader::downloadHeroesUsedAndRate(std::function<void(const QString &, int, double)> &func, const DataConfig &config)
{
	StatusBarSeter::setStatusBar("Downloading hero's used and rate data...");

	auto url = getHeroesUsedAndRateUrl(config);
	auto data = downloadWebPage(url);

	StatusBarSeter::setStatusBar("Download hero's used and rate data complete");

	StatusBarSeter::setStatusBar("Parsing...");

	WebDataSourceManager::getInstance().getWebDataSourceCurrent()->parse_HeroesUsedAndRate_WebPageData(func, data);

	StatusBarSeter::setStatusBar("Parse hero's used and rate data complete");

	return true;
}

bool WebDataDownloader::downloadHeroItems(const QString &heroname, std::function<void(const QString &, int, double, double)> &func, const DataConfig &config)
{
	StatusBarSeter::setStatusBar("Downloading items used and rate...");

	auto url = getHeroItemsUrl(heroname, config);
	auto data = downloadWebPage(url);

	StatusBarSeter::setStatusBar("Download items used and rate complete");

	StatusBarSeter::setStatusBar("Parsing...");

	WebDataSourceManager::getInstance().getWebDataSourceCurrent()->parse_HeroItems_WebPageData(func, data);

	StatusBarSeter::setStatusBar("Parse hero's used and rate data complete");

	return true;
}

QUrl WebDataDownloader::getHeroesUsedAndRateUrl(const DataConfig &config)
{
	return WebDataSourceManager::getInstance().getWebDataSourceCurrent()->getHeroesUsedAndRateUrl(config);
}

QUrl WebDataDownloader::getHeroItemsUrl(const QString &heroname, const DataConfig &config)
{
	return WebDataSourceManager::getInstance().getWebDataSourceCurrent()->getHeroItemsUrl(heroname, config);
}
