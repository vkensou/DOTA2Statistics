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

	m_playermatchhistory.downloadPlayerAllHistory(playerid);

	for (int i = 0; i < m_playermatchhistory.getCount(); ++i)
	{
		ui->listWidget->insertItem(ui->listWidget->count(), QString::number(m_playermatchhistory.at(i)));
	}
}

void PlayerMatchHistoryView::on_downloadMatches_clicked()
{
	downloadMatchDetail();
}


void PlayerMatchHistoryView::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
	int id = item->text().toInt();
	matchDblClicked(id);
}

void PlayerMatchHistoryView::downloadMatchDetail()
{
	int num = m_playermatchhistory.getCount();
	for (int i = 0; i < num; ++i)
	{
		int matchid = m_playermatchhistory.at(i);
		if (DataBaseManager::getInstance().isMatchSaved(matchid))
			continue;
		MatchDetail match(matchid);
		match.load();
		StatusBarSeter::setStatusBar(QString("Fetched player:%1, fetched match:%2").arg(m_fetchedplayer).arg(++m_fetchedmatch));
	}
}
