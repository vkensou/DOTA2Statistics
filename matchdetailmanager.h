#pragma once

#include "datamanagerbase.h"
#include "singleton.h"
#include "matchdetail.h"

class MatchDetailManager
	:public Singleton<MatchDetailManager>, public DataManagerBase<MatchDetail>
{
	MatchDetail &getMatchDetail(int matchid);
};