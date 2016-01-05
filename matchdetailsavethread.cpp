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
	while (!isInterruptionRequested())
	{
		auto match = MatchDetailParseThread::getInstance().getMatchDetail();
		if (match.matchid == 0)
			continue;

		qDebug() << "save match " << match.matchid;
		auto &dbmanager = DataBaseManager::getInstance();
		QMutexLocker locker(&dbmanager.getMutex());
		match.save();
		m_count++;
	}
}