#pragma once

#include "singleton.h"
#include <QUrl>
#include <functional>

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

	bool downloadHeroesUsedAndRate(std::function<void(const QString &, int, double)> &func, const DataConfig &config);
	bool downloadHeroItems(const QString &heroname, std::function<void(const QString &, int, double, double)> &func, const DataConfig &config);

private:
	QUrl getHeroesUsedAndRateUrl(const DataConfig &config);
	QUrl getHeroItemsUrl(const QString &heroname, const DataConfig &config);
};

