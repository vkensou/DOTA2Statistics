#pragma once

#include "singleton.h"
#include <QUrl>

class DataConfig;
class HeroesUsedAndRate;
class HeroItems;
class QString;

class WebDataDownloader
	:public Singleton<WebDataDownloader>
{
public:
	WebDataDownloader();
	~WebDataDownloader();

	bool downloadHeroesUsedAndRate(HeroesUsedAndRate &hur, const DataConfig &config);
	bool downloadHeroItems(HeroItems &hero, const DataConfig &config);

private:
	void parse_HeroesUsedAndRate_WebPageData(HeroesUsedAndRate &hur, const QString &webdata);
	void parse_HeroItems_WebPageData(HeroItems &hero, const QString &webdata);
	QUrl getHeroesUsedAndRateUrl(const DataConfig &config);
	QUrl getHeroItemsUrl(const QString &heroname, const DataConfig &config);
};

