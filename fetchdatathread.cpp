#include "fetchdatathread.h"

#include "databasemanager.h"
#include "statusbarsetter.h"
#include "playermatchhistory.h"
#include "fetchplayermatchhistorythread.h"
#include "matchdetaildownloadthread.h"
#include "matchdetailparsethread.h"
#include "matchdetailsavethread.h"
#include <QMutexLocker>

FetchDataThread::FetchDataThread()
{

}

unsigned int FetchDataThread::getStartTime()
{
	return m_starttime;
}

void FetchDataThread::setStartTime(unsigned int starttime)
{
	m_starttime = starttime;
}

void FetchDataThread::run()
{
	FetchPlayerMatchHistoryThread *m_fetchplayermatchhistorythread = new FetchPlayerMatchHistoryThread;
	MatchDetailDownloadThread *m_matchdetaildownloadthread = new MatchDetailDownloadThread;
	MatchDetailParseThread *m_matchdetailparsethread = new MatchDetailParseThread;
	MatchDetailSaveThread *m_matchdetailsavethread = new MatchDetailSaveThread;

	m_fetchplayermatchhistorythread->start();
	m_matchdetaildownloadthread->start();
	m_matchdetailparsethread->start();
	m_matchdetailsavethread->start();

	initFetchQueue();

	m_fetchedplayer = m_fetchedmatch = 0;
	while (!isInterruptionRequested())
	{
		QMutexLocker locker(&m_mutex);

		if (m_queuewaitfetchedplayers.isEmpty())
			initFetchQueue();
	}

	m_fetchplayermatchhistorythread->requestInterruption();
	m_matchdetaildownloadthread->requestInterruption();
	m_matchdetailparsethread->requestInterruption();
	m_matchdetailsavethread->requestInterruption();

	m_fetchplayermatchhistorythread->wait();
	m_matchdetaildownloadthread->wait();
	m_matchdetailparsethread->wait();
	m_matchdetailsavethread->wait();

	delete m_fetchplayermatchhistorythread;
	delete m_matchdetaildownloadthread;
	delete m_matchdetailparsethread;
	delete m_matchdetailsavethread;
}

int FetchDataThread::getPlayerid()
{
	QMutexLocker locker(&m_mutex);
	return m_queuewaitfetchedplayers.pop();
}

void FetchDataThread::pushPlayerid(int playerid)
{
	QMutexLocker locker(&m_mutex);
	m_queuewaitfetchedplayers.push(playerid);
}

int FetchDataThread::getCount()
{
	QMutexLocker locker(&m_mutex);
	return m_queuewaitfetchedplayers.getSize();
}

void FetchDataThread::initFetchQueue()
{
	int initplayerid = DataBaseManager::getInstance().getPlayerRandomly();
	m_queuewaitfetchedplayers.push(initplayerid);
}
