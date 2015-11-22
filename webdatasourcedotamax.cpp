#pragma execution_character_set("utf-8")

#include "webdatasourcedotamax.h"
#include "dataconfig.h"
#include <QStringList>
#include <algorithm>
#include <QDomDocument>
#include "utility.h"
#include "herolist.h"

bool WebDataSource_DotaMax::isSupportSetTime()
{
	return true;
}

bool WebDataSource_DotaMax::isSupportSetSkill()
{
	return true;
}

bool WebDataSource_DotaMax::isSupportSetMatchType()
{
	return true;
}

bool WebDataSource_DotaMax::isSupportSetServer()
{
	return true;
}

QUrl WebDataSource_DotaMax::getHeroesUsedAndRateUrl(const DataConfig &config)
{
	static const QString urlfmt = "http://dotamax.com/hero/rate/?&ladder=%1&skill=%2&time=%3&server=%4";
	return urlfmt.arg(getMatchTypeStr(config.matchtype)).arg(getSkillStr(config.skill)).arg(getTimeStr(config.time)).arg(getServerStr(config.server));
}

QUrl WebDataSource_DotaMax::getHeroItemsUrl(const QString &heroname, const DataConfig &config)
{
	static const QString heroitemsfmt = "http://dotamax.com/hero/detail/hero_items/%1/?&ladder=%2&skill=%3&time=%4&server=%5";
	QString fixedname = HeroList::getInstance().getDotaMaxName(heroname);
	return heroitemsfmt.arg(fixedname).arg(getMatchTypeStr(config.matchtype)).arg(getSkillStr(config.skill)).arg(getTimeStr(config.time)).arg(getServerStr(config.server));
}

const QStringList & WebDataSource_DotaMax::getTimeSetterTextList()
{
	static const QStringList list = { "全部", "本月", "本周", "6.85", "6.84", "6.83" };
	return list;
}

const QStringList & WebDataSource_DotaMax::getSkillSetterTextList()
{
	static const QStringList list = { "全部", "Normal", "High", "Very High", "职业比赛" };
	return list;
}

const QStringList & WebDataSource_DotaMax::getMatchTypeSetterTextList()
{
	static const QStringList list = { "全部", "普通", "天梯", "SOLO" };
	return list;
}

const QStringList & WebDataSource_DotaMax::getServerSetterTextList()
{
	static const QStringList list = { "全部", "中国", "国外" };
	return list;
}

const char * WebDataSource_DotaMax::getMatchTypeStr(unsigned char matchtype) const
{
	static const char *str[] =
	{
		"all",
		"y",
		"n",
		"solo"
	};
	return str[matchtype];
}

const char * WebDataSource_DotaMax::getSkillStr(unsigned char skill) const
{
	static const char *str[] =
	{
		"all",
		"n",
		"h",
		"vh",
		"pro"
	};
	return str[skill];
}

const char * WebDataSource_DotaMax::getTimeStr(unsigned char time) const
{
	static const char *str[] =
	{
		"all",
		"month",
		"week",
		"v685",
		"v684",
		"v683",
	};
	return str[time];
}

const char * WebDataSource_DotaMax::getServerStr(unsigned char server) const
{
	static const char *str[] =
	{
		"all",
		"cn",
		"world"
	};
	return str[server];
}

void WebDataSource_DotaMax::parse_HeroesUsedAndRate_WebPageData(std::function<void(const QString &, int, double) > func, const QString &webdata)
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
		float rate;
		int used;

		auto tdnode = node.firstChildElement();
		name = tdnode.firstChildElement("span").text();

		tdnode = tdnode.nextSiblingElement();
		rate = percentagetoFloat(tdnode.firstChildElement("div").text());

		tdnode = tdnode.nextSiblingElement();
		used = sepNumStrtoInt(tdnode.firstChildElement("div").text());

		func(name, used, rate);
	}
}

void WebDataSource_DotaMax::parse_HeroItems_WebPageData(std::function<void(const QString &, int, double, double) > func, const QString &webdata)
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

		func(name, used, rate, 0);
	}
}

QString WebDataSource_DotaMax::getFileParams(const DataConfig &config)
{
	static const QString urlfmt = "_%1_%2_%3_%4_dotamax";
	return urlfmt.arg(getMatchTypeStr(config.matchtype)).arg(getSkillStr(config.skill)).arg(getTimeStr(config.time)).arg(getServerStr(config.server));
}
