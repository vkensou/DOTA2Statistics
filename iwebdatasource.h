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

	virtual const QStringList & getTimeSetterTextList() = 0;
	virtual const QStringList & getSkillSetterText() = 0;
	virtual const QStringList & getMatchTypeSetterText() = 0;
	virtual const QStringList & getServerSetterText() = 0;
};