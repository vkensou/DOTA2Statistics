#pragma once

#include "idataview.h"
#include <QUrl>

namespace Ui
{
	class View_MatchDetail;
}

class QStyledItemDelegate;
class MatchDetail;

class MatchDetailView
	:public IDataView
{
	Q_OBJECT
public:
	MatchDetailView();
	~MatchDetailView();


	virtual int getType() override;

	virtual const QString & getViewName() override;

	virtual void on_DataSource_Changed() override;
public slots:
	void showMatchDetail(int matchid);
	void on_ShowMatchDetail_clicked();

private:
	void showData();

private:
	Ui::View_MatchDetail *ui;
	QStyledItemDelegate *itemimagedelegate;
	MatchDetail *m_match;
};

