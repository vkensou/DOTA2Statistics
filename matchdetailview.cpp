#pragma execution_character_set("utf-8")

#include "matchdetailview.h"

#include "ui_view_matchdetail.h"
#include "utility.h"
#include <QDomDocument>
#include <QTime>
#include <QDateTime>
#include "herolist.h"

const QString key = "387B6D180AD105C6CD289B0556C7A846";

MatchDetailView::MatchDetailView()
	:ui(new Ui::View_MatchDetail)
{
	ui->setupUi(this);
}

MatchDetailView::~MatchDetailView()
{
	delete ui;
}

void MatchDetailView::showMatchDetail(int matchid)
{
	auto url = getMatchDetailURL(matchid);
	auto data = downloadWebPage(url);
	parseMatchDetailData(data);
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

QUrl MatchDetailView::getMatchDetailURL(int matchid)
{
	static QString urlformat = "https://api.steampowered.com/IDOTA2Match_570/GetMatchDetails/V001/?key=%1&format=xml&match_id=%2";
	QString str = urlformat.arg(key).arg(matchid);

	QUrl url = str;
	return url;
}

void MatchDetailView::parseMatchDetailData(QString &data)
{
	QDomDocument doc;
	doc.setContent(data);

	auto root = doc.documentElement();

	for (auto node = root.firstChildElement(); !node.isNull(); node = node.nextSiblingElement())
	{
		if (node.tagName() == "radiant_win")
		{
			m_match.victoryparty = node.text() == "true";
		}
		else if (node.tagName() == "duration")
		{
			m_match.duration = node.text().toInt();
		}
		else if (node.tagName() == "start_time")
		{
			m_match.starttime = node.text().toLongLong();
		}
		else if (node.tagName() == "match_id")
		{
			m_match.matchid = node.text().toInt();
		}
		else if (node.tagName() == "match_seq_num")
		{
			m_match.matchseqnum = node.text().toInt();
		}
		else if (node.tagName() == "tower_status_radiant")
		{
			m_match.radianttower = node.text().toInt();
		}
		else if (node.tagName() == "tower_status_dire")
		{
			m_match.diretower = node.text().toInt();
		}
		else if (node.tagName() == "barracks_status_radiant")
		{
			m_match.radiantbarracks = node.text().toInt();
		}
		else if (node.tagName() == "barracks_status_dire")
		{
			m_match.direbarracks = node.text().toInt();
		}
		else if (node.tagName() == "cluster")
		{
			m_match.cluster = node.text().toInt();
		}
		else if (node.tagName() == "first_blood_time")
		{
			m_match.firstbloodtime = node.text().toInt();
		}
		else if (node.tagName() == "lobby_type")
		{
			m_match.lobbytype = node.text().toInt();
		}
		else if (node.tagName() == "human_players")
		{
			m_match.humanplaer = node.text().toInt();
		}
		else if (node.tagName() == "leagueid")
		{
			m_match.leagueid = node.text().toInt();
		}
		else if (node.tagName() == "positive_votes")
		{
			m_match.positivevotes = node.text().toInt();
		}
		else if (node.tagName() == "negative_votes")
		{
			m_match.negativevotes = node.text().toInt();
		}
		else if (node.tagName() == "game_mode")
		{
			m_match.gamemode = node.text().toInt();
		}
		else if (node.tagName() == "engine")
		{
			m_match.engine = node.text().toInt();
		}
		else if (node.tagName() == "players")
		{
			for (auto playernode = node.firstChildElement("player"); !playernode.isNull(); playernode = playernode.nextSiblingElement("player"))
			{
				auto slotnode = playernode.firstChildElement("player_slot");
				int slot = slotnode.text().toInt();
				MatchDetail::PlayerDetail *player;
				if (slot < 128)
					player = &m_match.radiantplayers[slot];
				else
					player = &m_match.direplayers[slot - 128];

				for (auto anode = playernode.firstChildElement(); !anode.isNull(); anode = anode.nextSiblingElement())
				{
					if (anode.tagName() == "account_id")
						player->accountid = anode.text().toInt();
					else if (anode.tagName() == "hero_id")
						player->heroid = anode.text().toInt();
					else if (anode.tagName() == "item_0")
						player->item0 = anode.text().toInt();
					else if (anode.tagName() == "item_1")
						player->item1 = anode.text().toInt();
					else if (anode.tagName() == "item_2")
						player->item2 = anode.text().toInt();
					else if (anode.tagName() == "item_3")
						player->item3 = anode.text().toInt();
					else if (anode.tagName() == "item_4")
						player->item4 = anode.text().toInt();
					else if (anode.tagName() == "item_5")
						player->item5 = anode.text().toInt();
					else if (anode.tagName() == "kills")
						player->kilss = anode.text().toInt();
					else if (anode.tagName() == "deaths")
						player->deaths = anode.text().toInt();
					else if (anode.tagName() == "assists")
						player->assists = anode.text().toInt();
					else if (anode.tagName() == "leaver_status")
						player->leaverstatus = anode.text().toInt();
					else if (anode.tagName() == "gold")
						player->gold = anode.text().toInt();
					else if (anode.tagName() == "last_hits")
						player->lasthits = anode.text().toInt();
					else if (anode.tagName() == "denies")
						player->denies = anode.text().toInt();
					else if (anode.tagName() == "gold_per_min")
						player->goldpermin = anode.text().toInt();
					else if (anode.tagName() == "xp_per_min")
						player->xppermin = anode.text().toInt();
					else if (anode.tagName() == "gold_spent")
						player->goldspent = anode.text().toInt();
					else if (anode.tagName() == "hero_damage")
						player->herodamage = anode.text().toInt();
					else if (anode.tagName() == "tower_damage")
						player->towerdamage = anode.text().toInt();
					else if (anode.tagName() == "hero_healing")
						player->herohealing = anode.text().toInt();
					else if (anode.tagName() == "level")
						player->level = anode.text().toInt();
					else if (anode.tagName() == "ability_upgrades")
					{

					}
					else if (anode.tagName() == "additional_units")
					{

					}
					else
					{
						QString tag = anode.tagName();
					}
				}
			}
		}
	}
}

void MatchDetailView::showData()
{
	ui->MatchID->setText(QString("比赛ID：%1").arg(m_match.matchid));
	ui->LeagueID->setText(QString("所属联赛ID：%1").arg(m_match.leagueid));

	QDateTime datetime = QDateTime::fromTime_t(m_match.starttime);
	ui->StartTime->setText(QString("开始时间：%1").arg(datetime.toString("yyyy.M.d H:mm")));

	QTime stime(0, 0, 0);
	auto dtime = stime.addSecs(m_match.duration);
	QString durastr;
	if (dtime.hour() > 0)
		durastr = dtime.toString("h:mm:ss");
	else
		durastr = dtime.toString("m:ss");
	ui->Duration->setText(QString("持续时间：%1").arg(durastr));

	auto fbtime = stime.addSecs(m_match.firstbloodtime);
	QString fbtstr;
	if (fbtime.hour() > 0)
		fbtstr = fbtime.toString("h:mm:ss");
	else
		fbtstr = fbtime.toString("m:ss");

	ui->FirstBloodTime->setText(QString("第一滴血：%1").arg(fbtstr));
	ui->LobbyType->setText(QString("游戏类型：%1").arg(m_match.lobbytype));
	ui->GameMode->setText(QString("模式：%1").arg(m_match.gamemode));
	ui->Skill->setText(QString("水平：%1").arg(0));
	if (m_match.victoryparty != 0)
		ui->VictoryParty->setText(QString("天辉获胜"));
	else
		ui->VictoryParty->setText(QString("夜魇获胜"));

	ui->PositiveVotes->setText(QString("赞：%1").arg(m_match.positivevotes));
	ui->NegativeVotes->setText(QString("踩：%1").arg(m_match.negativevotes));
	HeroList &herolist = HeroList::getInstance();

	for (int i = 0; i < 5; ++i)
	{
		ui->RadiantHeroList->setItem(i, 0, new QTableWidgetItem(QString::number(m_match.radiantplayers[i].accountid)));
		ui->RadiantHeroList->setItem(i, 1, new QTableWidgetItem(herolist.getChineseNameByID(m_match.radiantplayers[i].heroid)));
		ui->RadiantHeroList->setItem(i, 2, new QTableWidgetItem(QString::number(m_match.radiantplayers[i].level)));
		ui->RadiantHeroList->setItem(i, 3, new QTableWidgetItem(QString::number(m_match.radiantplayers[i].kilss)));
		ui->RadiantHeroList->setItem(i, 4, new QTableWidgetItem(QString::number(m_match.radiantplayers[i].deaths)));
		ui->RadiantHeroList->setItem(i, 5, new QTableWidgetItem(QString::number(m_match.radiantplayers[i].assists)));
		ui->RadiantHeroList->setItem(i, 6, new QTableWidgetItem(QString::number(m_match.radiantplayers[i].lasthits)));
		ui->RadiantHeroList->setItem(i, 7, new QTableWidgetItem(QString::number(m_match.radiantplayers[i].denies)));
		ui->RadiantHeroList->setItem(i, 8, new QTableWidgetItem(QString::number(m_match.radiantplayers[i].goldpermin)));
		ui->RadiantHeroList->setItem(i, 9, new QTableWidgetItem(QString::number(m_match.radiantplayers[i].xppermin)));
		ui->RadiantHeroList->setItem(i, 10, new QTableWidgetItem(QString::number(m_match.radiantplayers[i].herodamage)));
		ui->RadiantHeroList->setItem(i, 11, new QTableWidgetItem(QString::number(m_match.radiantplayers[i].towerdamage)));
		ui->RadiantHeroList->setItem(i, 12, new QTableWidgetItem(QString::number(m_match.radiantplayers[i].herohealing)));
	}

	for (int i = 0; i < 5; ++i)
	{
		ui->DireHeroList->setItem(i, 0, new QTableWidgetItem(QString::number(m_match.direplayers[i].accountid)));
		ui->DireHeroList->setItem(i, 1, new QTableWidgetItem(herolist.getChineseNameByID(m_match.direplayers[i].heroid)));
		ui->DireHeroList->setItem(i, 2, new QTableWidgetItem(QString::number(m_match.direplayers[i].level)));
		ui->DireHeroList->setItem(i, 3, new QTableWidgetItem(QString::number(m_match.direplayers[i].kilss)));
		ui->DireHeroList->setItem(i, 4, new QTableWidgetItem(QString::number(m_match.direplayers[i].deaths)));
		ui->DireHeroList->setItem(i, 5, new QTableWidgetItem(QString::number(m_match.direplayers[i].assists)));
		ui->DireHeroList->setItem(i, 6, new QTableWidgetItem(QString::number(m_match.direplayers[i].lasthits)));
		ui->DireHeroList->setItem(i, 7, new QTableWidgetItem(QString::number(m_match.direplayers[i].denies)));
		ui->DireHeroList->setItem(i, 8, new QTableWidgetItem(QString::number(m_match.direplayers[i].goldpermin)));
		ui->DireHeroList->setItem(i, 9, new QTableWidgetItem(QString::number(m_match.direplayers[i].xppermin)));
		ui->DireHeroList->setItem(i, 10, new QTableWidgetItem(QString::number(m_match.direplayers[i].herodamage)));
		ui->DireHeroList->setItem(i, 11, new QTableWidgetItem(QString::number(m_match.direplayers[i].towerdamage)));
		ui->DireHeroList->setItem(i, 12, new QTableWidgetItem(QString::number(m_match.direplayers[i].herohealing)));
	}
}
 