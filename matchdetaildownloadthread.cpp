#include "matchdetaildownloadthread.h"
#include <QUrl>
#include "fetchmatchhistorythread.h"
#include "Utility.h"
#include <QMutexLocker>
#include "databasemanager.h"

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

		auto match = FetchMatchHistoryThread::getInstance().getMatch();
		{
			if (match.first == 0)
				continue;
		}

		qDebug() << "download match " << match.first << " detail";
		auto &dbmanager = DataBaseManager::getInstance();

		{
			QMutexLocker locker(&dbmanager.getMutex());
			if (dbmanager.isMatchSaved(match.first))
			{
				dbmanager.updateMatchDetailSkill(match.first, match.second);
				continue;
			}
		}

		auto url = getMatchDetailURL(match.first);
		auto data = downloadWebPage(url);
		{
			QMutexLocker locker(&mutex);
			m_dataqueue.push(std::make_tuple(match.first, match.second, data));
		}
	}
	FetchMatchHistoryThread::getInstance().getMatch();
}

QUrl MatchDetailDownloadThread::getMatchDetailURL(int matchid)
{
	static QString urlformat = "https://api.steampowered.com/IDOTA2Match_570/GetMatchDetails/V001/?key=%1&format=xml&match_id=%2";
	QString str = urlformat.arg(key).arg(matchid);

	QUrl url = str;
	return url;

}

MatchDetailDownloadThread::MatchDownlodInfo MatchDetailDownloadThread::getData()
{
	QMutexLocker locker(&mutex);
	if (m_dataqueue.empty())
		return std::make_tuple(0, 0, "");

	auto data = m_dataqueue.front();
	m_dataqueue.pop();
	return data;
}

int MatchDetailDownloadThread::getCount()
{
	QMutexLocker locker(&mutex);
	return m_dataqueue.size();
}
