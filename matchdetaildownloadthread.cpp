#include "matchdetaildownloadthread.h"
#include <QUrl>
#include "fetchplayermatchhistorythread.h"
#include "Utility.h"
#include <QMutexLocker>

const QString key = "387B6D180AD105C6CD289B0556C7A846";

void MatchDetailDownloadThread::run()
{
	while (!isInterruptionRequested())
	{
		{
			QMutexLocker locker(&mutex);
			if (m_dataqueue.size() > MAX_SIZE)
				continue;
		}

		int matchid = FetchPlayerMatchHistoryThread::getInstance().getMatch();
		{
			if (matchid == 0)
				continue;
		}

		qDebug() << "download match " << matchid << " detail";
		auto url = getMatchDetailURL(matchid);
		auto data = downloadWebPage(url);
		{
			QMutexLocker locker(&mutex);
			m_dataqueue.push(data);
		}
	}
}

QUrl MatchDetailDownloadThread::getMatchDetailURL(int matchid)
{
	static QString urlformat = "https://api.steampowered.com/IDOTA2Match_570/GetMatchDetails/V001/?key=%1&format=xml&match_id=%2";
	QString str = urlformat.arg(key).arg(matchid);

	QUrl url = str;
	return url;

}

QString MatchDetailDownloadThread::getData()
{
	QMutexLocker locker(&mutex);
	if (m_dataqueue.empty())
		return "";

	QString data = m_dataqueue.front();
	m_dataqueue.pop();
	return data;
}

int MatchDetailDownloadThread::getCount()
{
	QMutexLocker locker(&mutex);
	return m_dataqueue.size();
}
