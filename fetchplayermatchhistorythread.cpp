#include "fetchplayermatchhistorythread.h"

#include <algorithm>
#include "fetchplayermatchhistorythread.h"
#include "fetchdatathread.h"
#include <QMutexLocker>
#include <QDebug>

int FetchPlayerMatchHistoryThread::getMatch()
{
	QMutexLocker locker(&mutex);
	if (!m_frontplayermatchhistory)
		return 0;

	if (m_frontcount == 0)
		return 0;

	if (m_frontindex >= m_frontcount)
		return 0;

	return m_frontplayermatchhistory->at(m_frontindex++);
}

int FetchPlayerMatchHistoryThread::getCount()
{
	return m_frontcount;
}

void FetchPlayerMatchHistoryThread::run()
{
	m_frontplayermatchhistory = new PlayerMatchHistory;
	m_backplayermatchhistory = new PlayerMatchHistory;
	m_needdownload = false;

	while (!isInterruptionRequested())
	{
		{
			QMutexLocker locker(&mutex);
			//当前的已用完，后备已准备好
			if ((m_frontcount == 0 || m_frontindex >= m_frontcount) && m_backplayermatchhistory->prepared())
			{
				std::swap(m_frontplayermatchhistory, m_backplayermatchhistory);
				m_frontindex = 0;
				m_frontcount = m_frontplayermatchhistory->getCount();
				m_needdownload = true;
			}
			else if (!m_backplayermatchhistory->prepared())
				m_needdownload = true;
		}
		if (m_needdownload)
		{
			int playerid = FetchDataThread::getInstance().getPlayerid();
			if (playerid == 0)
				continue;
			int starttime = FetchDataThread::getInstance().getStartTime();
			qDebug() << "fetch player:" << playerid << "'s match history";
			m_needdownload = false;
			m_backplayermatchhistory->downloadPlayerAllHistory(playerid, starttime);
		}
	}

	delete m_frontplayermatchhistory;
	delete m_backplayermatchhistory;
}
