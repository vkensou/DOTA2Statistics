#pragma execution_character_set("utf-8")

#include "webdatasourcedotabuff.h"

#include "dataconfig.h"
#include <QDomDocument>

bool WebDataSource_DotaBuff::isSupportSetTime()
{
	return true;
}

bool WebDataSource_DotaBuff::isSupportSetSkill()
{
	return false;
}

bool WebDataSource_DotaBuff::isSupportSetMatchType()
{
	return false;
}

bool WebDataSource_DotaBuff::isSupportSetServer()
{
	return false;
}

QUrl WebDataSource_DotaBuff::getHeroesUsedAndRateUrl(const DataConfig &config)
{
	static const QString urlfmt = "http://zh.dotabuff.com/heroes/played?date=%1";
	return urlfmt.arg(getTimeStr(config.time));
}

QUrl WebDataSource_DotaBuff::getHeroItemsUrl(const QString &heroname, const DataConfig &config)
{
	static const QString heroitemsfmt = "http://zh.dotabuff.com/heroes/%1/items?date=%2";
	QString nm = heroname;
	QString fixedname = nm.replace("_", "-");
	auto str = heroitemsfmt.arg(fixedname).arg(getTimeStr(config.time));
	return str;
}

const QStringList & WebDataSource_DotaBuff::getTimeSetterTextList()
{
	static const QStringList list = { "本月", "本周", "6.85b", "6.85", "6.84c" };
	return list;
}

const QStringList & WebDataSource_DotaBuff::getSkillSetterText()
{
	static const QStringList list = { "全部" };
	return list;
}

const QStringList & WebDataSource_DotaBuff::getMatchTypeSetterText()
{
	static const QStringList list = { "全部" };
	return list;
}

const QStringList & WebDataSource_DotaBuff::getServerSetterText()
{
	static const QStringList list = { "全部" };
	return list;
}

const char * WebDataSource_DotaBuff::getMatchTypeStr(unsigned char) const
{
	static const char *str = "all";
	return str;
}

const char * WebDataSource_DotaBuff::getSkillStr(unsigned char) const
{
	static const char *str = "all";
	return str;
}

const char * WebDataSource_DotaBuff::getTimeStr(unsigned char time) const
{
	static const char *str[] =
	{
		"month",
		"week",
		"patch_6.85b",
		"patch_6.85",
		"patch_6.84c",
	};
	return str[time];
}

const char * WebDataSource_DotaBuff::getServerStr(unsigned char) const
{
	static const char *str = "all";
	return str;
}

void WebDataSource_DotaBuff::parse_HeroesUsedAndRate_WebPageData(std::function<void(const QString &, int, double) > func, const QString &webdata)
{
	static QRegExp rx("<tbody>.*</tbody>");
	rx.indexIn(webdata);
	auto page = rx.cap(0);

	QDomDocument doc;
	doc.setContent(page);

	auto root = doc.documentElement();

	for (auto node = root.firstChildElement("tr"); !node.isNull(); node = node.nextSiblingElement())
	{
		QString name;
		int used;
		float rate;

		auto tdnode = node.firstChildElement("td");

		tdnode = tdnode.nextSiblingElement();
		name = tdnode.firstChildElement("a").text();

		tdnode = tdnode.nextSiblingElement();
		used = tdnode.attribute("data-value").toInt();

		tdnode = tdnode.nextSiblingElement();
		tdnode = tdnode.nextSiblingElement();
		rate = tdnode.attribute("data-value").toFloat();

		func(name, used, rate);
	}
}

void WebDataSource_DotaBuff::parse_HeroItems_WebPageData(std::function<void(const QString &, int, double, double) > func, const QString &webdata)
{
	static QRegExp rx("<tbody>.*</tbody>");
	rx.indexIn(webdata);
	auto page = rx.cap(0);

	QDomDocument doc;
	doc.setContent(page);

	auto root = doc.documentElement();

	for (auto node = root.firstChildElement("tr"); !node.isNull(); node = node.nextSiblingElement())
	{
		QString name;
		int used;
		float rate;

		auto tdnode = node.firstChildElement("td");

		tdnode = tdnode.nextSiblingElement();
		name = tdnode.firstChildElement("a").text();

		tdnode = tdnode.nextSiblingElement();
		used = tdnode.attribute("data-value").toInt();

		tdnode = tdnode.nextSiblingElement();
		rate = tdnode.attribute("data-value").toFloat();

		func(name, used, rate, 0);
	}
}

QString WebDataSource_DotaBuff::getFileParams(const DataConfig &config)
{
	static const QString urlfmt = "_%1_%2_%3_%4";
	return urlfmt.arg(getMatchTypeStr(config.matchtype)).arg(getSkillStr(config.skill)).arg(getTimeStr(config.time)).arg(getServerStr(config.server));
}
