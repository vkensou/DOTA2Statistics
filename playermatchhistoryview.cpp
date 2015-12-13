#pragma execution_character_set("utf-8")

#include "playermatchhistoryview.h"
#include "ui_view_playermatchhistory.h"
#include "utility.h"
#include <QDomDocument>
#include <algorithm>
#include "databasemanager.h"
#include "matchdetail.h"
#include "statusbarsetter.h"
#include "queuewaitfetchedplayers.h"

const QString key = "387B6D180AD105C6CD289B0556C7A846";

PlayerMatchHistoryView::PlayerMatchHistoryView(QWidget *parent)
	:IDataView(parent)
	, ui(new Ui::View_PlayerMatchHistory)
{
	ui->setupUi(this);
}

PlayerMatchHistoryView::~PlayerMatchHistoryView()
{
	delete ui;
}

int PlayerMatchHistoryView::getType()
{
	return IDataView::View_PlayerMatchHistory;
}

const QString & PlayerMatchHistoryView::getViewName()
{
	static QString name = "Íæ¼ÒÓÎÏ·¼ÇÂ¼";
	return name;
}

void PlayerMatchHistoryView::on_DataSource_Changed()
{

}

void PlayerMatchHistoryView::on_ShowHistory_clicked()
{
	ui->listWidget->clear();
	int playerid = ui->lineEdit->text().toInt();

	downloadPlayerAllHistory(playerid);

	for (int i = 0; i < m_matchhistory.size(); ++i)
	{
		ui->listWidget->insertItem(ui->listWidget->count(), QString::number(m_matchhistory[i]));
	}
}

void PlayerMatchHistoryView::on_downloadMatches_clicked()
{
	downloadMatchDetail();
}

void PlayerMatchHistoryView::on_fetchData_clicked()
{
	m_fetchstop = false;
	queuewaitfetchedplayers = new QueueWaitFetchedPlayers;

	int initplayerid = ui->lineEdit->text().toInt();
	queuewaitfetchedplayers->push(initplayerid);

	const int maxfetch = 100;
	m_fetchedplayer = m_fetchedmatch = 0;
	while (!queuewaitfetchedplayers->isEmpty() && m_fetchedplayer++ < maxfetch && !m_fetchstop)
	{
		int pid = queuewaitfetchedplayers->pop();
		DataBaseManager::getInstance().updataPlayerStatus(pid, 1);
		downloadPlayerAllHistory(pid);
		downloadMatchDetail();
		DataBaseManager::getInstance().updataPlayerStatus(pid, 2);
		StatusBarSeter::setStatusBar(QString("Fetched player:%1, fetched match:%2").arg(m_fetchedplayer).arg(m_fetchedmatch));
	}

	delete queuewaitfetchedplayers;
	StatusBarSeter::setStatusBar(QString("Fetched Over. Fetched player:%1, fetched match:%2").arg(m_fetchedplayer-1).arg(m_fetchedmatch));
}

void PlayerMatchHistoryView::on_stopFetch_clicked()
{
	m_fetchstop = true;
}

void PlayerMatchHistoryView::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
	int id = item->text().toInt();
	matchDblClicked(id);
}

QUrl PlayerMatchHistoryView::getMatchHistoryURL(int playerid, int startmatch)
{
	static QString urlformat = "https://api.steampowered.com/IDOTA2Match_570/GetMatchHistory/V001/?key=%1&format=xml";
	static QString option1 = "&account_id=%1";
	static QString option2 = "&start_at_match_id=%1";

	QString str = urlformat.arg(key);
	if (playerid)
		str += option1.arg(playerid);
	if (startmatch)
		str += option2.arg(startmatch);

	QUrl url = str;
	return url;
}

void PlayerMatchHistoryView::parseHistoryData(QString &data, int &remaining, int &lastmatch)
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
		m_matchhistory.push_back(id);
	}
	lastmatch = m_matchhistory.back();
}

void PlayerMatchHistoryView::downloadPlayerAllHistory(int playerid)
{
	m_matchhistory.clear();

	int remaining = 0, lastmatch = 0;
	do
	{
		auto url = getMatchHistoryURL(playerid, lastmatch);
		auto data = downloadWebPage(url);
		parseHistoryData(data, remaining, lastmatch);
	} while (remaining > 0 && !m_fetchstop);

	std::sort(m_matchhistory.begin(), m_matchhistory.end());
	auto pos = std::unique(m_matchhistory.begin(), m_matchhistory.end());
	m_matchhistory.erase(pos, m_matchhistory.end());
	qDebug() << m_matchhistory.size();
}

void PlayerMatchHistoryView::downloadMatchDetail()
{
	int num = m_matchhistory.size();
	for (int i = 0; i < num && !m_fetchstop; ++i)
	{
		int matchid = m_matchhistory[i];
		if (DataBaseManager::getInstance().isMatchSaved(matchid))
			continue;
		MatchDetail match(matchid);
		match.load();
		StatusBarSeter::setStatusBar(QString("Fetched player:%1, fetched match:%2").arg(m_fetchedplayer).arg(++m_fetchedmatch));
	}
}
