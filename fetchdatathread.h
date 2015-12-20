#pragma once

#include <QThread>
#include <QMutex>
#include "singleton.h"
#include "queuewaitfetchedplayers.h"

class FetchDataThread
	:public QThread, public Singleton<FetchDataThread>
{
public:
	FetchDataThread();

	unsigned int getStartTime();
	void setStartTime(unsigned int starttime);
	virtual void run() override;
	int getPlayerid();
	void pushPlayerid(int playerid);
	int getCount();

public:
	QMutex m_mutex;

private:
	void initFetchQueue();

private:
	int m_fetchedplayer, m_fetchedmatch;
	QueueWaitFetchedPlayers m_queuewaitfetchedplayers;
	int m_starttime;
};