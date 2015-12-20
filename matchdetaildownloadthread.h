#pragma once

#include <QThread>
#include <queue>
#include <QMutex>
#include "singleton.h"

class MatchDetailDownloadThread
	:public QThread, public Singleton<MatchDetailDownloadThread>
{
public:
	virtual void run() override;
	QString getData();
	int getCount();

private:
	QUrl getMatchDetailURL(int matchid);

private:
	QMutex mutex;
	const int MAX_SIZE{ 10 };
	std::queue<QString> m_dataqueue;
};