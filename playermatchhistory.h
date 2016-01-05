#pragma once

#include <vector>
#include <QUrl>
#include <QString>

class PlayerMatchHistory
{
public:
	int getCount();
	int at(int i);
	bool downloadPlayerAllHistory(int playerid, unsigned int starttime = 0, int skill = 0);
	bool prepared();
	void clear();

private:
	QUrl getMatchHistoryURL(int playerid = 0, int startmatch = 0, int skill = 0, unsigned int startdate = 0, int gamemode = 0);
	void parseHistoryData(QString &data, int starttime, int &remaining, int &lastmatch);

private:
	std::vector<int> m_matchhistory;
	bool m_prepared{ false };
};