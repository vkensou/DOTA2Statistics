#include "fetchdatathread.h"

#include "databasemanager.h"
#include "statusbarsetter.h"
#include "playermatchhistory.h"
#include "fetchmatchhistorythread.h"
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
	FetchMatchHistoryThread *m_fetchplayermatchhistorythread = new FetchMatchHistoryThread;
	MatchDetailDownloadThread *m_matchdetaildownloadthread = new MatchDetailDownloadThread;
	MatchDetailParseThread *m_matchdetailparsethread = new MatchDetailParseThread;
	MatchDetailSaveThread *m_matchdetailsavethread = new MatchDetailSaveThread;

	m_fetchplayermatchhistorythread->start();
	m_matchdetaildownloadthread->start();
	m_matchdetailparsethread->start();
	m_matchdetailsavethread->start();

	m_fetchedplayer = m_fetchedmatch = 0;
	while (!isInterruptionRequested())
	{
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
