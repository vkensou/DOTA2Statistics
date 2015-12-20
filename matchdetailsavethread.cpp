#include "matchdetailsavethread.h"

#include "matchdetailparsethread.h"
#include <QDebug>
#include "databasemanager.h"

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
		DataBaseManager::getInstance().lock();
		match.save();
		DataBaseManager::getInstance().unlock();
		m_count++;
	}
}
