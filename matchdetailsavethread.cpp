#include "matchdetailsavethread.h"

#include "matchdetailparsethread.h"
#include <QDebug>
#include "databasemanager.h"

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
		DataBaseManager::getInstance().transaction();
		match.save();
		DataBaseManager::getInstance().commit();
		m_count++;
	}
}
