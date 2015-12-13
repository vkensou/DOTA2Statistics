#include "matchdetailmanager.h"

MatchDetail & MatchDetailManager::getMatchDetail(int matchid)
{
	QString key = QString::number(matchid);

	std::function<MatchDetail*()> func = [matchid]()
	{
		return new MatchDetail(matchid);
	};

	return getDatum(key, func);
}
