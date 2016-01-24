#include "matchdetaildownloadthread.h"
#include <QUrl>
#include "fetchmatchhistorythread.h"
#include <QMutexLocker>
#include "downloadcenter.h"

const QString key = "387B6D180AD105C6CD289B0556C7A846";

MatchDetailDownloadThread::MatchDetailDownloadThread()
{
}

void MatchDetailDownloadThread::run()
{
	while (!isInterruptionRequested())
	{
		{
			QMutexLocker locker(&mutex);
			if (m_dataqueue.size() > MAX_SIZE)
				continue;
		}

		auto match = FetchMatchHistoryThread::getMatch();
		{
			if (match.first == 0)
				continue;
		}

		qDebug() << "begin download match " << match.first << " detail";

		auto url = getMatchDetailURL(match.first);
		QNetworkReply::NetworkError error(QNetworkReply::NoError);
		QString data = DownloadCenter::getInstance().download(*this, url, error, 0, 1);
		if (!error)
			push(std::make_tuple(match.first, match.second, data));
	}
	FetchMatchHistoryThread::free();
}

QUrl MatchDetailDownloadThread::getMatchDetailURL(int matchid)
{
	static QString urlformat = "https://api.steampowered.com/IDOTA2Match_570/GetMatchDetails/V001/?key=%1&format=xml&match_id=%2";
	QString str = urlformat.arg(key).arg(matchid);

	QUrl url = str;
	return url;
}

void MatchDetailDownloadThread::push(MatchDownlodInfo &info)
{
	QMutexLocker locker(&mutex);
	m_dataqueue.push(info);
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

QMutex MatchDetailDownloadThread::mutex;
std::queue<MatchDetailDownloadThread::MatchDownlodInfo> MatchDetailDownloadThread::m_dataqueue;
