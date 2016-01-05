#pragma once

#include <QThread>
#include <queue>
#include "singleton.h"
#include "matchdetail.h"
#include <queue>
#include <QMutex>

class MatchDetailParseThread
	:public QThread, public Singleton<MatchDetailParseThread>
{
public:
	virtual void run() override;
	MatchDetail getMatchDetail();
	int getCount();

private:
	QMutex mutex;
	std::queue<MatchDetail> m_queuematchdetail;
	const int MAX_SIZE{ 10 };
};