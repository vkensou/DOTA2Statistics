#pragma once

#include <QThread>
#include "playermatchhistory.h"
#include <QMutex>
#include "singleton.h"
#include <queue>
#include <QSemaphore>
#include <utility>

class FetchMatchHistoryThread
	:public QThread, public Singleton<FetchMatchHistoryThread>
{
public:
	std::pair<int, int> getMatch();
	int getCount();
	int getSkill();

public:
	QMutex mutex;

private:
	virtual void run() override;

private:
	PlayerMatchHistory *m_frontplayermatchhistory{ nullptr }, *m_backplayermatchhistory{ nullptr };
	int m_frontcount{ 0 }, m_frontindex{ 0 };
	bool m_needdownload;
	int m_skill{ 1 };
	int m_frontskill, m_backskill;
};