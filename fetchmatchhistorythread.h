#pragma once

#include <QThread>
#include <QMutex>
#include "singleton.h"
#include <queue>
#include <utility>
#include <QSemaphore>

class QDomElement;

class FetchMatchHistoryThread
	:public QThread
{
public:
	typedef std::pair<int, int> MatchIDAndSkill;
	void init(int skill);
	static MatchIDAndSkill getMatch();
	static void free();
	static int getCount();

private:
	virtual void run() override;
	int downloadAllHistory(int skill);
	QUrl getMatchHistoryURL(int playerid = 0, int startmatch = 0, int skill = 0, unsigned int startdate = 0, int gamemode = 0);
	int parseHistoryData(QString &data, int skill, int starttime, int &remaining, int &lastmatch);
	void push(MatchIDAndSkill &match);
	bool isNeed(QDomElement &node);

private:
	int m_skill{ 0 };
	int m_waittime{ 0 };
	static const int MAX_SIZE{ 1500 };
	static std::queue<MatchIDAndSkill> m_queue;
	static QSemaphore freesmp, usedsmp;
	static QMutex mutex;
};