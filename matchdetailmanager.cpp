#include "matchdetailmanager.h"

MatchDetail & MatchDetailManager::getMatchDetail(int matchid)
{
	QString key = QString::number(matchid);

	std::function<MatchDetail*()> func = []()
	{
		return new MatchDetail;
	};

	return getDatum(key, func);
}
