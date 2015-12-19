#pragma execution_character_set("utf-8")

#include "matchdetailview.h"

#include "ui_view_matchdetail.h"
#include "utility.h"
#include <QDomDocument>
#include <QTime>
#include <QDateTime>
#include "herolist.h"
#include "ItemImageDelegate.h"
#include "abilitieslist.h"
#include "matchdetail.h"
#include "matchdetailmanager.h"

const QString key = "387B6D180AD105C6CD289B0556C7A846";

MatchDetailView::MatchDetailView()
	:ui(new Ui::View_MatchDetail)
	, itemimagedelegate(new ItemImageDelegate)
{
	ui->setupUi(this);
	ui->RadiantHeroList->setItemDelegateForColumn(13, itemimagedelegate);
	ui->RadiantHeroList->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui->RadiantHeroList->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

	ui->RadiantHeroList->setColumnWidth(0, 70);
	ui->RadiantHeroList->setColumnWidth(1, 67);
	ui->RadiantHeroList->setColumnWidth(2, 32);
	ui->RadiantHeroList->setColumnWidth(3, 32);
	ui->RadiantHeroList->setColumnWidth(4, 32);
	ui->RadiantHeroList->setColumnWidth(5, 32);
	ui->RadiantHeroList->setColumnWidth(6, 32);
	ui->RadiantHeroList->setColumnWidth(7, 32);
	ui->RadiantHeroList->setColumnWidth(8, 68);
	ui->RadiantHeroList->setColumnWidth(9, 68);
	ui->RadiantHeroList->setColumnWidth(10, 56);
	ui->RadiantHeroList->setColumnWidth(11, 56);
	ui->RadiantHeroList->setColumnWidth(12, 56);
	ui->RadiantHeroList->setColumnWidth(13, 205);
	ui->RadiantHeroList->setColumnWidth(14, 202);

	ui->DireHeroList->setItemDelegateForColumn(13, itemimagedelegate);
	ui->DireHeroList->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui->DireHeroList->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

	ui->DireHeroList->setColumnWidth(0, 70);
	ui->DireHeroList->setColumnWidth(1, 67);
	ui->DireHeroList->setColumnWidth(2, 32);
	ui->DireHeroList->setColumnWidth(3, 32);
	ui->DireHeroList->setColumnWidth(4, 32);
	ui->DireHeroList->setColumnWidth(5, 32);
	ui->DireHeroList->setColumnWidth(6, 32);
	ui->DireHeroList->setColumnWidth(7, 32);
	ui->DireHeroList->setColumnWidth(8, 68);
	ui->DireHeroList->setColumnWidth(9, 68);
	ui->DireHeroList->setColumnWidth(10, 56);
	ui->DireHeroList->setColumnWidth(11, 56);
	ui->DireHeroList->setColumnWidth(12, 56);
	ui->DireHeroList->setColumnWidth(13, 205);
	ui->DireHeroList->setColumnWidth(14, 202);

}

MatchDetailView::~MatchDetailView()
{
	delete ui;
	delete itemimagedelegate;
}

void MatchDetailView::showMatchDetail(int matchid)
{
	auto &manager = MatchDetailManager::getInstance();
	m_match = &manager.getMatchDetail(matchid);
	m_match->load(false);
	showData();
}

int MatchDetailView::getType()
{
	return IDataView::View_MatchDetail;
}

const QString & MatchDetailView::getViewName()
{
	static QString name = "比赛详情";
	return name;
}

void MatchDetailView::on_DataSource_Changed()
{
}

void MatchDetailView::on_ShowMatchDetail_clicked()
{
	showMatchDetail(ui->lineEdit->text().toInt());
}

void MatchDetailView::showData()
{
	ui->MatchID->setText(QString("比赛ID：%1").arg(m_match->matchid));
	ui->LeagueID->setText(QString("所属联赛ID：%1").arg(m_match->leagueid));

	QDateTime datetime = QDateTime::fromTime_t(m_match->starttime);
	ui->StartTime->setText(QString("开始时间：%1").arg(datetime.toString("yyyy.M.d H:mm")));

	QTime stime(0, 0, 0);
	auto dtime = stime.addSecs(m_match->duration);
	QString durastr;
	if (dtime.hour() > 0)
		durastr = dtime.toString("h:mm:ss");
	else
		durastr = dtime.toString("m:ss");
	ui->Duration->setText(QString("持续时间：%1").arg(durastr));

	auto fbtime = stime.addSecs(m_match->firstbloodtime);
	QString fbtstr;
	if (fbtime.hour() > 0)
		fbtstr = fbtime.toString("h:mm:ss");
	else
		fbtstr = fbtime.toString("m:ss");

	ui->FirstBloodTime->setText(QString("第一滴血：%1").arg(fbtstr));
	ui->LobbyType->setText(QString("游戏类型：%1").arg(m_match->lobbytype));
	ui->GameMode->setText(QString("模式：%1").arg(m_match->gamemode));
	ui->Skill->setText(QString("水平：%1").arg(0));
	if (m_match->victoryparty != 0)
		ui->VictoryParty->setText(QString("天辉获胜"));
	else
		ui->VictoryParty->setText(QString("夜魇获胜"));

	ui->PositiveVotes->setText(QString("赞：%1").arg(m_match->positivevotes));
	ui->NegativeVotes->setText(QString("踩：%1").arg(m_match->negativevotes));
	HeroList &herolist = HeroList::getInstance();
	AbilitiesList &abilitieslist = AbilitiesList::getInstance();
	for (int i = 0; i < 5; ++i)
	{
		ui->RadiantHeroList->setItem(i, 0, new QTableWidgetItem(QString::number(m_match->radiantplayers[i].accountid)));
		ui->RadiantHeroList->setItem(i, 1, new QTableWidgetItem(herolist.getChineseNameByID(m_match->radiantplayers[i].heroid)));
		ui->RadiantHeroList->setItem(i, 2, new QTableWidgetItem(QString::number(m_match->radiantplayers[i].level)));
		ui->RadiantHeroList->setItem(i, 3, new QTableWidgetItem(QString::number(m_match->radiantplayers[i].kills)));
		ui->RadiantHeroList->setItem(i, 4, new QTableWidgetItem(QString::number(m_match->radiantplayers[i].deaths)));
		ui->RadiantHeroList->setItem(i, 5, new QTableWidgetItem(QString::number(m_match->radiantplayers[i].assists)));
		ui->RadiantHeroList->setItem(i, 6, new QTableWidgetItem(QString::number(m_match->radiantplayers[i].lasthits)));
		ui->RadiantHeroList->setItem(i, 7, new QTableWidgetItem(QString::number(m_match->radiantplayers[i].denies)));
		ui->RadiantHeroList->setItem(i, 8, new QTableWidgetItem(QString::number(m_match->radiantplayers[i].goldpermin)));
		ui->RadiantHeroList->setItem(i, 9, new QTableWidgetItem(QString::number(m_match->radiantplayers[i].xppermin)));
		ui->RadiantHeroList->setItem(i, 10, new QTableWidgetItem(QString::number(m_match->radiantplayers[i].herodamage)));
		ui->RadiantHeroList->setItem(i, 11, new QTableWidgetItem(QString::number(m_match->radiantplayers[i].towerdamage)));
		ui->RadiantHeroList->setItem(i, 12, new QTableWidgetItem(QString::number(m_match->radiantplayers[i].herohealing)));
		QTableWidgetItem *heroitems = new QTableWidgetItem;
		heroitems->setData(0, QVariant::fromValue(ItemList(m_match->radiantplayers[i].item[0].id, m_match->radiantplayers[i].item[1].id, m_match->radiantplayers[i].item[2].id, m_match->radiantplayers[i].item[3].id, m_match->radiantplayers[i].item[4].id, m_match->radiantplayers[i].item[5].id)));
		ui->RadiantHeroList->setItem(i, 13, heroitems);

		QString str = "";
		for (int j = 0; j < 25; ++j)
		{
			int id = m_match->radiantplayers[i].abilityupgrades[j].ability;
			if (id == 0)
				break;
			str += abilitieslist.getIndexByID(id);
		}

		ui->RadiantHeroList->setItem(i, 14, new QTableWidgetItem(str));
	}

	for (int i = 0; i < 5; ++i)
	{
		ui->DireHeroList->setItem(i, 0, new QTableWidgetItem(QString::number(m_match->direplayers[i].accountid)));
		ui->DireHeroList->setItem(i, 1, new QTableWidgetItem(herolist.getChineseNameByID(m_match->direplayers[i].heroid)));
		ui->DireHeroList->setItem(i, 2, new QTableWidgetItem(QString::number(m_match->direplayers[i].level)));
		ui->DireHeroList->setItem(i, 3, new QTableWidgetItem(QString::number(m_match->direplayers[i].kills)));
		ui->DireHeroList->setItem(i, 4, new QTableWidgetItem(QString::number(m_match->direplayers[i].deaths)));
		ui->DireHeroList->setItem(i, 5, new QTableWidgetItem(QString::number(m_match->direplayers[i].assists)));
		ui->DireHeroList->setItem(i, 6, new QTableWidgetItem(QString::number(m_match->direplayers[i].lasthits)));
		ui->DireHeroList->setItem(i, 7, new QTableWidgetItem(QString::number(m_match->direplayers[i].denies)));
		ui->DireHeroList->setItem(i, 8, new QTableWidgetItem(QString::number(m_match->direplayers[i].goldpermin)));
		ui->DireHeroList->setItem(i, 9, new QTableWidgetItem(QString::number(m_match->direplayers[i].xppermin)));
		ui->DireHeroList->setItem(i, 10, new QTableWidgetItem(QString::number(m_match->direplayers[i].herodamage)));
		ui->DireHeroList->setItem(i, 11, new QTableWidgetItem(QString::number(m_match->direplayers[i].towerdamage)));
		ui->DireHeroList->setItem(i, 12, new QTableWidgetItem(QString::number(m_match->direplayers[i].herohealing)));
		QTableWidgetItem *heroitems = new QTableWidgetItem;
		heroitems->setData(0, QVariant::fromValue(ItemList(m_match->direplayers[i].item[0].id, m_match->direplayers[i].item[1].id, m_match->direplayers[i].item[2].id, m_match->direplayers[i].item[3].id, m_match->direplayers[i].item[4].id, m_match->direplayers[i].item[5].id)));
		ui->DireHeroList->setItem(i, 13, heroitems);

		QString str = "";
		for (int j = 0; j < 25; ++j)
		{
			int id = m_match->direplayers[i].abilityupgrades[j].ability;
			if (id == 0)
				break;
			str += abilitieslist.getIndexByID(id);
		}

		ui->DireHeroList->setItem(i, 14, new QTableWidgetItem(str));
	}
}
 