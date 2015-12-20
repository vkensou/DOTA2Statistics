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
	MatchDetailDownloadThread *m_matchdetaildownloadthread = new MatchDetailDownloadThread[4];

	MatchDetailParseThread *m_matchdetailparsethread = new MatchDetailParseThread;
	MatchDetailSaveThread *m_matchdetailsavethread = new MatchDetailSaveThread;

	m_fetchplayermatchhistorythread->start();
	for (int i = 0; i < 4; ++i)
		m_matchdetaildownloadthread[i].start();
	m_matchdetailparsethread->start();
	m_matchdetailsavethread->start();

	m_fetchedplayer = m_fetchedmatch = 0;
	while (!isInterruptionRequested())
	{
	}

	m_fetchplayermatchhistorythread->requestInterruption();
	for (int i = 0; i < 4; ++i)
		m_matchdetaildownloadthread[i].requestInterruption();
	m_matchdetailparsethread->requestInterruption();
	m_matchdetailsavethread->requestInterruption();

	m_fetchplayermatchhistorythread->wait();
	for (int i = 0; i < 4; ++i)
		m_matchdetaildownloadthread[i].wait();
	m_matchdetailparsethread->wait();
	m_matchdetailsavethread->wait();

	delete m_fetchplayermatchhistorythread;
	delete[] m_matchdetaildownloadthread;
	delete m_matchdetailparsethread;
	delete m_matchdetailsavethread;
}
