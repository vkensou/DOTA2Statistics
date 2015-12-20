#include "matchdetailparsethread.h"
#include <QUrl>
#include "fetchmatchhistorythread.h"
#include "Utility.h"
#include "matchdetaildownloadthread.h"
#include <QMutexLocker>

const QString key = "387B6D180AD105C6CD289B0556C7A846";

void MatchDetailParseThread::run()
{
	while (!isInterruptionRequested())
	{
		{
			QMutexLocker locker(&mutex);
			if (m_queuematchdetail.size() > MAX_SIZE)
				continue;
		}

		auto data = MatchDetailDownloadThread::getData();
		if (std::get<2>(data).isEmpty())
			continue;

		qDebug() << "parse match " << std::get<0>(data) << " detail";
		MatchDetail matchdetail(std::get<0>(data), std::get<1>(data));
		matchdetail.parseMatchDetailData(std::get<2>(data));

		{
			QMutexLocker locker(&mutex);
			m_queuematchdetail.push(matchdetail);
		}
	}
}

MatchDetail MatchDetailParseThread::getMatchDetail()
{
	QMutexLocker locker(&mutex);
	if (m_queuematchdetail.empty())
		return MatchDetail(0);

	MatchDetail match = m_queuematchdetail.front();
	m_queuematchdetail.pop();
	return match;
}

int MatchDetailParseThread::getCount()
{
	QMutexLocker locker(&mutex);
	return m_queuematchdetail.size();
}
