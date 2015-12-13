#pragma once

#include "idataview.h"
#include <QUrl>
#include <vector>

namespace Ui
{
	class View_PlayerMatchHistory;
}
class QListWidgetItem;
class QueueWaitFetchedPlayers;

class PlayerMatchHistoryView
	:public IDataView
{
	Q_OBJECT
public:
	PlayerMatchHistoryView(QWidget *parent = nullptr);
	~PlayerMatchHistoryView();

	virtual int getType() override;

	virtual const QString & getViewName() override;

	virtual void on_DataSource_Changed() override;

public slots:
	void on_ShowHistory_clicked();
	void on_downloadMatches_clicked();
	void on_fetchData_clicked();
	void on_stopFetch_clicked();
	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

signals:
	void matchDblClicked(int matchid);

private:
	QUrl getMatchHistoryURL(int playerid = 0, int startmatch = 0);
	void parseHistoryData(QString &data, int &remaining, int &lastmatch);
	void downloadPlayerAllHistory(int playerid);
	void downloadMatchDetail();

private:
	Ui::View_PlayerMatchHistory *ui;
	std::vector<int> m_matchhistory;
	QueueWaitFetchedPlayers *queuewaitfetchedplayers;
	bool m_fetchstop;
	int m_fetchedplayer, m_fetchedmatch;
};