#pragma once

#include <QThread>
#include <QMutex>
#include "singleton.h"
#include <queue>
#include <utility>
#include <QSemaphore>

class FetchMatchHistoryThread
	:public QThread, public Singleton<FetchMatchHistoryThread>
{
public:
	std::pair<int, int> getMatch();
	int getCount();

private:
	virtual void run() override;
	bool downloadAllHistory(int skill);
	QUrl getMatchHistoryURL(int playerid = 0, int startmatch = 0, int skill = 0, unsigned int startdate = 0, int gamemode = 0);
	void parseHistoryData(QString &data, int skill, int starttime, int &remaining, int &lastmatch);

private:
	int m_skill{ 1 };
	int m_frontskill, m_backskill;
	const int MAX_SIZE{ 1500 };
	std::queue<std::pair<int, int>> m_queue;
	QSemaphore freesmp{ MAX_SIZE }, usedsmp;
	bool m_prepared{ false };
	QMutex mutex;
};