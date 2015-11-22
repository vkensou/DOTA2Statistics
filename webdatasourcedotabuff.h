#pragma once
#include "iwebdatasource.h"

class WebDataSource_DotaBuff
	:public IWebDataSource
{


	virtual bool isSupportSetTime() override;

	virtual bool isSupportSetSkill() override;

	virtual bool isSupportSetMatchType() override;

	virtual bool isSupportSetServer() override;

	virtual QUrl getHeroesUsedAndRateUrl(const DataConfig &config) override;

	virtual QUrl getHeroItemsUrl(const QString &heroname, const DataConfig &config) override;

	virtual const QStringList & getTimeSetterTextList() override;

	virtual const QStringList & getSkillSetterTextList() override;

	virtual const QStringList & getMatchTypeSetterTextList() override;

	virtual const QStringList & getServerSetterTextList() override;

	virtual const char * getMatchTypeStr(unsigned char matchtype) const override;

	virtual const char * getSkillStr(unsigned char skill) const override;

	virtual const char * getTimeStr(unsigned char time) const override;

	virtual const char * getServerStr(unsigned char server) const override;

	virtual void parse_HeroesUsedAndRate_WebPageData(std::function<void(const QString &, int, double) > &func, const QString &webdata) override;

	virtual void parse_HeroItems_WebPageData(std::function<void(const QString &, int, double, double) > &func, const QString &webdata) override;

	virtual QString getFileParams(const DataConfig &config) override;

};