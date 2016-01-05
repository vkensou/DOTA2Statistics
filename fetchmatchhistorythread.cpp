#include "fetchmatchhistorythread.h"

#include <algorithm>
#include "fetchmatchhistorythread.h"
#include "fetchdatathread.h"
#include <QMutexLocker>
#include <QDebug>
#include "Utility.h"
#include <QDomDocument>

const QString key = "387B6D180AD105C6CD289B0556C7A846";

void FetchMatchHistoryThread::init(int skill)
{
	assert(skill >= 0 && skill <= 3);
	m_skill = skill;
}

std::pair<int, int> FetchMatchHistoryThread::getMatch()
{
	usedsmp.acquire();
	QMutexLocker locker(&mutex);
	if (m_queue.empty())
		return{ 0, 0 };

	auto data = m_queue.front();
	m_queue.pop();
	freesmp.release();
	return data;
}

void FetchMatchHistoryThread::free()
{
	freesmp.release(100);
}

int FetchMatchHistoryThread::getCount()
{
	QMutexLocker locker(&mutex);
	return m_queue.size();
}

void FetchMatchHistoryThread::run()
{
	while (!isInterruptionRequested())
	{
		downloadAllHistory(m_skill);
	}
}

bool FetchMatchHistoryThread::downloadAllHistory(int skill)
{
	int remaining = 0, lastmatch = 0;
	do
	{
		auto url = getMatchHistoryURL(0, lastmatch, skill);
		int error = 0;
		auto data = downloadWebPage(url, &error);
		if (error != 0)
			return false;
		parseHistoryData(data, skill, 0, remaining, lastmatch);
	} while (remaining > 0 && !isInterruptionRequested());
	return true;
}

QUrl FetchMatchHistoryThread::getMatchHistoryURL(int playerid /*= 0*/, int startmatch /*= 0*/, int skill /*= 0*/, unsigned int startdate /*= 0*/, int gamemode /*= 0*/)
{
	static QString urlformat = "https://api.steampowered.com/IDOTA2Match_570/GetMatchHistory/V001/?key=%1&format=xml";
	static QString option1 = "&account_id=%1";
	static QString option2 = "&start_at_match_id=%1";
	static QString option3 = "&skill=%1";
	static QString option4 = "&game_mode=%1";
	static QString option5 = "&date_min=%1";

	QString str = urlformat.arg(key);
	if (playerid)
		str += option1.arg(playerid);
	if (startmatch)
		str += option2.arg(startmatch);
	if (skill)
		str += option3.arg(skill);
	if (startdate)
		str += option4.arg(startdate);
	if (gamemode)
		str += option5.arg(gamemode);

	QUrl url = str;
	return url;
}

void FetchMatchHistoryThread::parseHistoryData(QString &data, int skill, int starttime, int &remaining, int &lastmatch)
{
	QDomDocument doc;
	doc.setContent(data);

	auto root = doc.documentElement();
	remaining = root.firstChildElement("results_remaining").text().toInt();
	auto matchnodes = root.firstChildElement("matches");
	int last = 0;
	for (auto matchnode = matchnodes.firstChildElement("match"); !matchnode.isNull() && !isInterruptionRequested(); matchnode = matchnode.nextSiblingElement("match"))
	{
		auto idnode = matchnode.firstChildElement("match_id");
		int id = idnode.text().toInt();
		if (matchnode.firstChildElement("start_time").text().toInt() > starttime)
		{
			if (isNeed(matchnode))
				push(std::make_pair(id, skill));
		}
		else
		{
			remaining = 0;
			break;
		}
		last = id;
	}
	lastmatch = last;
}

void FetchMatchHistoryThread::push(MatchIDAndSkill &match)
{
	freesmp.acquire();
	QMutexLocker locker(&mutex);
	m_queue.push(match);
	usedsmp.release();
}

bool FetchMatchHistoryThread::isNeed(QDomElement &node)
{
	auto lobbynode = node.firstChildElement("lobby_type");
	if (!lobbynode.isNull())
	{
		int lobby = lobbynode.text().toInt();
		if (lobby == 7)
			return true;
	}
	return false;
}

std::queue<FetchMatchHistoryThread::MatchIDAndSkill> FetchMatchHistoryThread::m_queue;
QSemaphore FetchMatchHistoryThread::freesmp{ FetchMatchHistoryThread::MAX_SIZE }, FetchMatchHistoryThread::usedsmp;
QMutex FetchMatchHistoryThread::mutex;