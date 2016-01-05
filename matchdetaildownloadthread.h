#pragma once

#include <QThread>
#include <queue>
#include <QMutex>
#include "singleton.h"

class MatchDetailDownloadThread
	:public QThread
{
	typedef std::tuple<int, int, QString> MatchDownlodInfo;
public:

	virtual void run() override;
	static MatchDownlodInfo getData();
	static int getCount();

private:
	QUrl getMatchDetailURL(int matchid);
	static void push(MatchDownlodInfo &info);
private:
	static QMutex mutex;
	static const int MAX_SIZE{ 10 };
	static std::queue<MatchDownlodInfo> m_dataqueue;
};