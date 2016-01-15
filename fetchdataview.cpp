#pragma execution_character_set("utf-8")

#include "fetchdataview.h"
#include "ui_view_fetchdata.h"
#include <QUrl>
#include "fetchdatathread.h"
#include "fetchmatchhistorythread.h"
#include "matchdetaildownloadthread.h"
#include "matchdetailparsethread.h"
#include "matchdetailsavethread.h"
#include "downloadcenter.h"
#include <QTimerEvent>

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
	return IDataView::View_FetchData;
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
	if (m_fetchthread)
		return;

	auto datetime = ui->dtedit->dateTime();
	auto starttime = datetime.toTime_t();
	int gamemode = 22;

	m_fetchthread = new FetchDataThread;
	m_fetchthread->setStartTime(starttime);
	connect(m_fetchthread, SIGNAL(ready()), this, SLOT(on_fetchthread_ready()));
	m_fetchthread->start();
}

void FetchDataView::on_stop_clicked()
{
	if (!m_fetchthread)
		return;
	if (m_timer)
		killTimer(m_timer);
	m_fetchthread->requestInterruption();
	m_fetchthread->wait();
	delete m_fetchthread;
	m_fetchthread = 0;
}

void FetchDataView::on_fetchthread_ready()
{
	m_timer = startTimer(1);
}

QUrl FetchDataView::getMatchHistoryURL(int playerid /*= 0*/, int startmatch /*= 0*/, int skill /*= 0*/, unsigned int startdate /*= 0*/, int gamemode /*= 0*/)
{
	static QString urlformat = "https://api.steampowered.com/IDOTA2Match_570/GetMatchHistory/V001/?key=%1&format=xml";
	static QString option1 = "&account_id=%1";
	static QString option2 = "&start_at_match_id=%1";
	static QString option3 = "&skill=%1";
	static QString option4 = "&game_mode=%1";
	static QString option5 = "&date_min=%1";

	QString str = urlformat.arg(key);
	if (playerid)
		str += option1.arg(playerid);
	if (startmatch)
		str += option2.arg(startmatch);
	if (skill)
		str += option3.arg(skill);
	if (startdate)
		str += option4.arg(startdate);
	if (gamemode)
		str += option5.arg(gamemode);

	QUrl url = str;
	return url;
}

void FetchDataView::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_timer)
	{
		ui->numofhistoryqueue->setText(QString::number(FetchMatchHistoryThread::getCount()));
		ui->numofdownloadqueue->setText(QString::number(MatchDetailDownloadThread::getCount()));
		ui->numofparsequeue->setText(QString::number(MatchDetailParseThread::getInstance().getCount()));
		ui->numoffetched->setText(QString::number(MatchDetailSaveThread::getInstance().getSaved()));
		ui->numoferror->setText(QString::number(DownloadCenter::getInstance().getNumOfError()));
	}
}

void FetchDataView::closeEvent(QCloseEvent *)
{
	on_stop_clicked();
}
