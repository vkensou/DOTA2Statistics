#include "webdatadownloader.h"
#include "statusbarsetter.h"
#include "Utility.h"
#include <QDomDocument>
#include "heroesusedandrate.h"
#include "heroitems.h"
#include "dataconfig.h"

WebDataDownloader::WebDataDownloader()
{
}


WebDataDownloader::~WebDataDownloader()
{
}

bool WebDataDownloader::downloadHeroesUsedAndRate(HeroesUsedAndRate &hur, const DataConfig &config)
{
	StatusBarSeter::setStatusBar("Downloading hero's used and rate data...");

	hur.clear();

	auto url = getHeroesUsedAndRateUrl(config);
	auto data = downloadWebPage(url);

	StatusBarSeter::setStatusBar("Download hero's used and rate data complete");

	parse_HeroesUsedAndRate_WebPageData(hur, data);

	return true;
}

bool WebDataDownloader::downloadHeroItems(HeroItems &hero, const DataConfig &config)
{
	StatusBarSeter::setStatusBar("Downloading items used and rate...");

	hero.clear();

	auto url = getHeroItemsUrl(hero.getName(), config);
	auto data = downloadWebPage(url);

	StatusBarSeter::setStatusBar("Download items used and rate complete");

	parse_HeroItems_WebPageData(hero, data);

	return true;
}

void WebDataDownloader::parse_HeroesUsedAndRate_WebPageData(HeroesUsedAndRate &hur, const QString &webdata)
{
	StatusBarSeter::setStatusBar("Parsing...");

	static QRegExp rx("<tbody>.*</tbody>");
	rx.indexIn(webdata);
	auto page = rx.cap(0);

	QDomDocument doc;
	doc.setContent(page);

	auto root = doc.documentElement();

	for (auto node = root.firstChildElement("tr"); !node.isNull(); node = node.nextSiblingElement())
	{
		QString name;
		float rate;
		int used;

		auto tdnode = node.firstChildElement();
		name = tdnode.firstChildElement("span").text();

		tdnode = tdnode.nextSiblingElement();
		rate = percentagetoFloat(tdnode.firstChildElement("div").text());

		tdnode = tdnode.nextSiblingElement();
		used = sepNumStrtoInt(tdnode.firstChildElement("div").text());

		hur.addHero(name, used, rate);
	}

	StatusBarSeter::setStatusBar("Parse hero's used and rate data complete");
}

void WebDataDownloader::parse_HeroItems_WebPageData(HeroItems &hero, const QString &webdata)
{
	StatusBarSeter::setStatusBar("Parsing...");

	static QRegExp rx("<tbody>.*</tbody>");
	rx.indexIn(webdata);
	auto page = rx.cap(0);

	QDomDocument doc;
	doc.setContent(page);

	auto root = doc.documentElement();

	for (auto node = root.firstChildElement("tr"); !node.isNull(); node = node.nextSiblingElement())
	{
		QString name;
		float rate;
		int used;

		auto tdnode = node.firstChildElement();
		name = tdnode.firstChildElement("a").text();
		name.replace(" ", "");
		name.replace("\n", "");

		tdnode = tdnode.nextSiblingElement();
		used = sepNumStrtoInt(tdnode.firstChildElement("div").text());

		tdnode = tdnode.nextSiblingElement();
		rate = percentagetoFloat(tdnode.firstChildElement("div").text());

		hero.addItem(name, used, rate, 0);
	}

	StatusBarSeter::setStatusBar("Parse complete");
}

QUrl WebDataDownloader::getHeroesUsedAndRateUrl(const DataConfig &config)
{
	static const QString urlfmt = "http://dotamax.com/hero/rate/?%1";
	return urlfmt.arg(config.getUrlParams());
}

QUrl WebDataDownloader::getHeroItemsUrl(const QString &heroname, const DataConfig &config)
{
	static const QString heroitemsfmt = "http://dotamax.com/hero/detail/hero_items/%1/?%2";
	return heroitemsfmt.arg(heroname).arg(config.getUrlParams());
}
