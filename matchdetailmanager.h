#pragma once

#include "datamanagerbase.h"
#include "singleton.h"
#include "matchdetail.h"

class MatchDetailManager
	:public Singleton<MatchDetailManager>, public DataManagerBase<MatchDetail>
{
public:
	MatchDetail &getMatchDetail(int matchid);
};