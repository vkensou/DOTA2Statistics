#pragma execution_character_set("utf-8")

#include "playermatchhistoryview.h"
#include "ui_view_playermatchhistory.h"
#include "utility.h"
#include <QDomDocument>

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
	m_matchhistory.clear();
	ui->listWidget->clear();
	int playid = ui->lineEdit->text().toInt();
	auto url = getMatchHistoryURL(playid);
	auto data = downloadWebPage(url);
	parseHistoryData(data);

	for (int i = 0; i < m_matchhistory.size(); ++i)
	{
		ui->listWidget->insertItem(ui->listWidget->count(), QString::number(m_matchhistory[i]));
	}
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

void PlayerMatchHistoryView::parseHistoryData(QString &data)
{
	QDomDocument doc;
	doc.setContent(data);

	auto root = doc.documentElement();
	auto matchnodes = root.firstChildElement("matches");

	for (auto matchnode = matchnodes.firstChildElement("match"); !matchnode.isNull(); matchnode = matchnode.nextSiblingElement("match"))
	{
		auto idnode = matchnode.firstChildElement("match_id");
		int id = idnode.text().toInt();
		m_matchhistory.push_back(id);
	}
}
