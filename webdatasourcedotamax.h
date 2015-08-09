#pragma once

#include "iwebdatasource.h"

class WebDataSource_DotaMax
	:public IWebDataSource
{
public:
	bool isSupportSetTime() override;
	bool isSupportSetSkill() override;
	bool isSupportSetMatchType() override;
	bool isSupportSetServer() override;

	QUrl getHeroesUsedAndRateUrl(const DataConfig &config) override;
	QUrl getHeroItemsUrl(const QString &heroname, const DataConfig &config) override;

	virtual const QStringList & getTimeSetterTextList() override;
	virtual const QStringList & getSkillSetterText() override;
	virtual const QStringList & getMatchTypeSetterText() override;
	virtual const QStringList & getServerSetterText() override;
};