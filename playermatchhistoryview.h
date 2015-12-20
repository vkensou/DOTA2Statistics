#pragma once

#include "idataview.h"
#include <QUrl>
#include <vector>
#include "playermatchhistory.h"

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
	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

signals:
	void matchDblClicked(int matchid);

private:
	void downloadMatchDetail();

private:
	Ui::View_PlayerMatchHistory *ui;
	PlayerMatchHistory m_playermatchhistory;
	QueueWaitFetchedPlayers *queuewaitfetchedplayers;
	int m_fetchedplayer, m_fetchedmatch;
};