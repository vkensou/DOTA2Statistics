#pragma execution_character_set("utf-8")

#include "playermatchhistoryview.h"
#include "ui_view_playermatchhistory.h"

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

}
