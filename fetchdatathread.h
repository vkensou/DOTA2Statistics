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

public:
	QMutex m_mutex;

private:
	int m_fetchedplayer, m_fetchedmatch;
	int m_starttime;
};