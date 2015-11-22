#pragma once

#include <QUrl>
#include <QStringList>
#include <functional>

class DataConfig;
class QString;

class IWebDataSource
{
public:
	virtual ~IWebDataSource(){}

	virtual bool isSupportSetTime() = 0;
	virtual bool isSupportSetSkill() = 0;
	virtual bool isSupportSetMatchType() = 0;
	virtual bool isSupportSetServer() = 0;

	virtual QUrl getHeroesUsedAndRateUrl(const DataConfig &config) = 0;
	virtual QUrl getHeroItemsUrl(const QString &heroname, const DataConfig &config) = 0;
	virtual QString getFileParams(const DataConfig &config) = 0;

	virtual const QStringList & getTimeSetterTextList() = 0;
	virtual const QStringList & getSkillSetterTextList() = 0;
	virtual const QStringList & getMatchTypeSetterTextList() = 0;
	virtual const QStringList & getServerSetterTextList() = 0;

	virtual const char * getMatchTypeStr(unsigned char matchtype) const = 0;
	virtual const char * getSkillStr(unsigned char skill) const = 0;
	virtual const char * getTimeStr(unsigned char time) const = 0;
	virtual const char * getServerStr(unsigned char server) const = 0;

	virtual void parse_HeroesUsedAndRate_WebPageData(std::function<void(const QString &, int, double)> &func, const QString &webdata) = 0;
	virtual void parse_HeroItems_WebPageData(std::function<void(const QString &, int, double, double)> &func, const QString &webdata) = 0;

};