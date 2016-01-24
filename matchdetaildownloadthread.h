#pragma once

#include "needdownload.h"
#include <queue>
#include <QMutex>
#include "singleton.h"
#include <QtNetwork>

class MatchDetailDownloadThread
	:public NeedDownload
{
	Q_OBJECT
	typedef std::tuple<int, int, QString> MatchDownlodInfo;
public:
	MatchDetailDownloadThread();

	virtual void run() override;
	static MatchDownlodInfo getData();
	static int getCount();

private slots:

private:
	QUrl getMatchDetailURL(int matchid);
	static void push(MatchDownlodInfo &info);
private:
	static QMutex mutex;
	static const int MAX_SIZE{ 10 };
	static std::queue<MatchDownlodInfo> m_dataqueue;
};