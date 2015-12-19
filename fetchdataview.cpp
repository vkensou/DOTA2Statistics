#pragma execution_character_set("utf-8")

#include "fetchdataview.h"
#include "ui_view_fetchdata.h"
#include <QUrl>

const QString key = "387B6D180AD105C6CD289B0556C7A846";

FetchDataView::FetchDataView(QWidget *parent /*= nullptr*/)
	:IDataView(parent)
	, ui(new Ui::View_FetchData)
{
	ui->setupUi(this);
}

FetchDataView::~FetchDataView()
{
	delete ui;
}

int FetchDataView::getType()
{
	return IDataView::View_FetchDAta;
}

const QString & FetchDataView::getViewName()
{
	static QString name = "°ÇÈ¡Êý¾Ý";
	return name;
}

void FetchDataView::on_DataSource_Changed()
{

}

void FetchDataView::on_fetchdata_clicked()
{
	auto datetime = ui->dtedit->dateTime();
	auto starttime = datetime.toTime_t();
	int gamemode = 22;
}

QUrl FetchDataView::getMatchHistoryURL(int playerid /*= 0*/, int startmatch /*= 0*/, int skill /*= 0*/, unsigned int startdate /*= 0*/, int gamemode /*= 0*/)
{
	static QString urlformat = "https://api.steampowered.com/IDOTA2Match_570/GetMatchHistory/V001/?key=%1&format=xml";
	static QString option1 = "&account_id=%1";
	static QString option2 = "&start_at_match_id=%1";
	static QString option3 = "&skill=%1";
	static QString option4 = "&game_mode=%1";
	static QString option4 = "&date_min=%1";

	QString str = urlformat.arg(key);
	if (playerid)
		str += option1.arg(playerid);
	if (startmatch)
		str += option2.arg(startmatch);
	if (skill)
		str += option3.arg(skill);
	if (startdate)
		str += option3.arg(startdate);
	if (gamemode)
		str += option3.arg(gamemode);

	QUrl url = str;
	return url;
}
