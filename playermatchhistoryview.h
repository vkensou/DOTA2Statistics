#pragma once

#include "idataview.h"

namespace Ui
{
	class View_PlayerMatchHistory;
}
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

private:
	Ui::View_PlayerMatchHistory *ui;

};