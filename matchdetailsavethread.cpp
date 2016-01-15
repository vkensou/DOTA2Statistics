#include "matchdetailsavethread.h"

#include "matchdetailparsethread.h"
#include <QDebug>
#include "databasemanager.h"
#include <QMutexLocker>

int MatchDetailSaveThread::getSaved()
{
	return m_count;
}

int MatchDetailSaveThread::getCount()
{
	return m_count;
}

void MatchDetailSaveThread::run()
{
	m_count = 0;
	DataBaseManager::getInstance().transaction();
	int saved = 0;
	while (!isInterruptionRequested())
	{
		auto match = MatchDetailParseThread::getInstance().getMatchDetail();
		if (match.matchid == 0)
			continue;

		qDebug() << "save match " << match.matchid;
		match.save(false, true);
		m_count++;
		saved++;
		if (saved > 10)
		{
			DataBaseManager::getInstance().commit();
			saved = 0;
			DataBaseManager::getInstance().transaction();
		}
	}
	DataBaseManager::getInstance().commit();
}
