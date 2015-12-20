#include "playermatchhistory.h"

#include "Utility.h"
#include <QDomDocument>

const QString key = "387B6D180AD105C6CD289B0556C7A846";

int PlayerMatchHistory::getCount()
{
	return m_matchhistory.size();
}

int PlayerMatchHistory::at(int i)
{
	return m_matchhistory[i];
}

bool PlayerMatchHistory::downloadPlayerAllHistory(int playerid, unsigned int starttime)
{
	m_prepared = false;
	m_matchhistory.clear();

	int remaining = 0, lastmatch = 0;
	do
	{
		auto url = getMatchHistoryURL(playerid, lastmatch, 0);
		int error = 0;
		auto data = downloadWebPage(url, &error);
		if (error != 0)
			return false;
		parseHistoryData(data, starttime, remaining, lastmatch);
	} while (remaining > 0);

	std::sort(m_matchhistory.begin(), m_matchhistory.end());
	auto pos = std::unique(m_matchhistory.begin(), m_matchhistory.end());
	m_matchhistory.erase(pos, m_matchhistory.end());
	qDebug() << "player match history count:" << m_matchhistory.size();
	m_prepared = true;
	return true;
}

bool PlayerMatchHistory::prepared()
{
	return m_prepared;
}

QUrl PlayerMatchHistory::getMatchHistoryURL(int playerid /*= 0*/, int startmatch /*= 0*/, int skill /*= 0*/, unsigned int startdate /*= 0*/, int gamemode /*= 0*/)
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

void PlayerMatchHistory::parseHistoryData(QString &data, int starttime, int &remaining, int &lastmatch)
{
	QDomDocument doc;
	doc.setContent(data);

	auto root = doc.documentElement();
	remaining = root.firstChildElement("results_remaining").text().toInt();
	auto matchnodes = root.firstChildElement("matches");

	for (auto matchnode = matchnodes.firstChildElement("match"); !matchnode.isNull(); matchnode = matchnode.nextSiblingElement("match"))
	{
		auto idnode = matchnode.firstChildElement("match_id");
		int id = idnode.text().toInt();
		if (matchnode.firstChildElement("start_time").text().toInt() > starttime)
			m_matchhistory.push_back(id);
		else
		{
			remaining = 0;
			break;
		}
	}
	lastmatch = m_matchhistory.back();
}
