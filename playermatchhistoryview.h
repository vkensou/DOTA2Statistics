#pragma once

#include "idataview.h"
#include <QUrl>
#include <vector>

namespace Ui
{
	class View_PlayerMatchHistory;
}
class QListWidgetItem;

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
	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

signals:
	void matchDblClicked(int matchid);

private:
	QUrl getMatchHistoryURL(int playerid = 0, int startmatch = 0);
	void parseHistoryData(QString &data);

private:
	Ui::View_PlayerMatchHistory *ui;
	std::vector<int> m_matchhistory;
};