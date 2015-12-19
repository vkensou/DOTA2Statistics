#pragma once

#include "idataview.h"

namespace Ui
{
	class View_FetchData;
}

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

private:
	QUrl getMatchHistoryURL(int playerid = 0, int startmatch = 0, int skill = 0, unsigned int startdate = 0, int gamemode = 0);

private:
	Ui::View_FetchData *ui;
};