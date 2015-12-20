#include "fetchmatchhistorythread.h"

#include <algorithm>
#include "fetchmatchhistorythread.h"
#include "fetchdatathread.h"
#include <QMutexLocker>
#include <QDebug>

std::pair<int, int> FetchMatchHistoryThread::getMatch()
{
	QMutexLocker locker(&mutex);
	if (!m_frontplayermatchhistory)
		return{ 0, 0 };

	if (m_frontindex >= m_frontcount)
		return{ 0, 0 };

	return{ m_frontplayermatchhistory->at(m_frontindex++), m_frontskill };
}

int FetchMatchHistoryThread::getCount()
{
	return m_frontcount;
}

int FetchMatchHistoryThread::getSkill()
{
	return m_frontskill;
}

void FetchMatchHistoryThread::run()
{
	m_frontplayermatchhistory = new PlayerMatchHistory;
	m_backplayermatchhistory = new PlayerMatchHistory;
	m_needdownload = false;
	m_frontskill = m_backskill = m_skill = 1;
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
				m_frontskill = m_backskill;
				m_frontplayermatchhistory->clear();
			}
			if (!m_backplayermatchhistory->prepared())
				m_needdownload = true;
		}
		if (m_needdownload)
		{
			//int playerid = FetchDataThread::getInstance().getPlayerid();
			//if (playerid == 0)
			//	continue;
			int starttime = FetchDataThread::getInstance().getStartTime();
			qDebug() << "fetch skill:" << m_skill << " match history";
			m_needdownload = false;
			m_backplayermatchhistory->downloadPlayerAllHistory(0, starttime, m_skill);
			m_backskill = m_skill;
			m_skill++;
			if (m_skill > 3)
				m_skill = 1;
		}
	}

	delete m_frontplayermatchhistory;
	delete m_backplayermatchhistory;
}
