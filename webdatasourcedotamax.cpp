#pragma execution_character_set("utf-8")

#include "webdatasourcedotamax.h"
#include "dataconfig.h"
#include <QStringList>
#include <algorithm>

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
	static const QString urlfmt = "http://dotamax.com/hero/rate/?%1";
	return urlfmt.arg(config.getUrlParams());
}

QUrl WebDataSource_DotaMax::getHeroItemsUrl(const QString &heroname, const DataConfig &config)
{
	static const QString heroitemsfmt = "http://dotamax.com/hero/detail/hero_items/%1/?%2";
	return heroitemsfmt.arg(heroname).arg(config.getUrlParams());
}

const QStringList & WebDataSource_DotaMax::getTimeSetterTextList()
{
	static const QStringList list = { "ȫ��", "����", "����", "6.84", "6.83" ,"6.82"};
	return list;
}

const QStringList & WebDataSource_DotaMax::getSkillSetterText()
{
	static const QStringList list = { "ȫ��", "Normal", "High", "Very High", "ְҵ����" };
	return list;
}

const QStringList & WebDataSource_DotaMax::getMatchTypeSetterText()
{
	static const QStringList list = { "ȫ��", "��ͨ", "����", "SOLO" };
	return list;
}

const QStringList & WebDataSource_DotaMax::getServerSetterText()
{
	static const QStringList list = { "ȫ��", "�й�", "����" };
	return list;
}
