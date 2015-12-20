#pragma once

#include <QThread>
#include "playermatchhistory.h"
#include <QMutex>
#include "singleton.h"

class FetchPlayerMatchHistoryThread
	:public QThread, public Singleton<FetchPlayerMatchHistoryThread>
{
public:
	int getMatch();
	int getCount();

public:
	QMutex mutex;

private:
	virtual void run() override;

private:
	PlayerMatchHistory *m_frontplayermatchhistory{ nullptr }, *m_backplayermatchhistory{ nullptr };
	int m_frontcount{ 0 }, m_frontindex{ 0 };
	bool m_needdownload;
};