#include "fetchdatathread.h"

#include "databasemanager.h"
#include "statusbarsetter.h"
#include "playermatchhistory.h"
#include "fetchmatchhistorythread.h"
#include "matchdetaildownloadthread.h"
#include "matchdetailparsethread.h"
#include "matchdetailsavethread.h"
#include <QMutexLocker>
#include "downloadcenter.h"

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
	int const HISTORY_THREAD_COUNT = 3, DETAIL_THREAD_COUNT = 3;
	DownloadCenter *downloadcenter = new DownloadCenter();
	FetchMatchHistoryThread *m_fetchplayermatchhistorythread = new FetchMatchHistoryThread[HISTORY_THREAD_COUNT];
	MatchDetailDownloadThread *m_matchdetaildownloadthread = new MatchDetailDownloadThread[DETAIL_THREAD_COUNT];
	MatchDetailParseThread *m_matchdetailparsethread = new MatchDetailParseThread;
	MatchDetailSaveThread *m_matchdetailsavethread = new MatchDetailSaveThread;

	emit ready();

	for (int i = 0; i < HISTORY_THREAD_COUNT; ++i)
	{
		m_fetchplayermatchhistorythread[i].init(i+1);
		m_fetchplayermatchhistorythread[i].start();
	}
	for (int i = 0; i < DETAIL_THREAD_COUNT; ++i)
	{
		m_matchdetaildownloadthread[i].start();
	}
	m_matchdetailparsethread->start();
	m_matchdetailsavethread->start();

	m_fetchedplayer = m_fetchedmatch = 0;
	while (!isInterruptionRequested())
	{
		sleep(1);
	}

	for (int i = 0; i < HISTORY_THREAD_COUNT; ++i)
		m_fetchplayermatchhistorythread[i].requestInterruption();
	for (int i = 0; i < DETAIL_THREAD_COUNT; ++i)
		m_matchdetaildownloadthread[i].requestInterruption();
	m_matchdetailparsethread->requestInterruption();
	m_matchdetailsavethread->requestInterruption();

	for (int i = 0; i < HISTORY_THREAD_COUNT; ++i)
		m_fetchplayermatchhistorythread[i].wait();
	for (int i = 0; i < DETAIL_THREAD_COUNT; ++i)
		m_matchdetaildownloadthread[i].wait();
	m_matchdetailparsethread->wait();
	m_matchdetailsavethread->wait();

	delete[] m_fetchplayermatchhistorythread;
	delete[] m_matchdetaildownloadthread;
	delete m_matchdetailparsethread;
	delete m_matchdetailsavethread;
	delete downloadcenter;
}
