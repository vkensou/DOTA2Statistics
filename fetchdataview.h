#pragma once

#include "idataview.h"

namespace Ui
{
	class View_FetchData;
}
class FetchDataThread;

class FetchDataView
	:public IDataView
{
	Q_OBJECT
public:
	FetchDataView(QWidget *parent = nullptr);
	~FetchDataView();

	virtual int getType() override;

	virtual const QString & getViewName() override;

	virtual void on_DataSource_Changed() override;

public slots:
	void on_fetchdata_clicked();
	void on_stop_clicked();

private:
	QUrl getMatchHistoryURL(int playerid = 0, int startmatch = 0, int skill = 0, unsigned int startdate = 0, int gamemode = 0);

	virtual void timerEvent(QTimerEvent *) override;

	virtual void closeEvent(QCloseEvent *) override;

private:
	Ui::View_FetchData *ui;
	FetchDataThread *m_fetchthread{ nullptr };
	int m_timer{ 0 };
};