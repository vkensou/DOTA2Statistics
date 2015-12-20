#pragma once

#include <QThread>
#include <queue>
#include <QMutex>
#include "singleton.h"

class MatchDetailDownloadThread
	:public QThread, public Singleton<MatchDetailDownloadThread>
{
	typedef std::tuple<int, int, QString> MatchDownlodInfo;
public:

	virtual void run() override;
	MatchDownlodInfo getData();
	int getCount();

private:
	QUrl getMatchDetailURL(int matchid);

private:
	QMutex mutex;
	const int MAX_SIZE{ 10 };
	std::queue<MatchDownlodInfo> m_dataqueue;
};