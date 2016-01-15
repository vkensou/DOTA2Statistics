#include "matchdetail.h"
#include "utility.h"
#include <QDomDocument>
#include "databasemanager.h"

const QString key = "387B6D180AD105C6CD289B0556C7A846";

MatchDetail::MatchDetail(int matchid, int skill) 
	:matchid(matchid), skill(skill)
{

}

bool MatchDetail::loaded()
{
	return m_init;
}

bool MatchDetail::loadFromDataBase()
{
	return DataBaseManager::getInstance().loadMatchDetail(*this);
}

void MatchDetail::download()
{
	auto url = getMatchDetailURL(matchid);
	auto data = downloadWebPage(url);
	parseMatchDetailData(data);

	m_init = true;
}

void MatchDetail::save(bool transaction /*= true*/, bool lock /*= false*/)
{
	DataBaseManager::getInstance().saveMatchDetail(*this, transaction, lock);
}

void MatchDetail::clear()
{
	m_init = false;
}

QUrl MatchDetail::getMatchDetailURL(int matchid)
{
	static QString urlformat = "https://api.steampowered.com/IDOTA2Match_570/GetMatchDetails/V001/?key=%1&format=xml&match_id=%2";
	QString str = urlformat.arg(key).arg(matchid);

	QUrl url = str;
	return url;
}


void MatchDetail::parseMatchDetailData(QString &data)
{
	QDomDocument doc;
	doc.setContent(data);

	auto root = doc.documentElement();

	for (auto node = root.firstChildElement(); !node.isNull(); node = node.nextSiblingElement())
	{
		if (node.tagName() == "match_id")
		{
			if (matchid == 0)
				matchid = node.text().toInt();
		}
		else if (node.tagName() == "radiant_win")
		{
			victoryparty = node.text() == "true" ? 1 : 0;
		}
		else if (node.tagName() == "duration")
		{
			duration = node.text().toInt();
		}
		else if (node.tagName() == "start_time")
		{
			starttime = node.text().toLongLong();
		}
		else if (node.tagName() == "match_id")
		{
			matchid = node.text().toInt();
		}
		else if (node.tagName() == "match_seq_num")
		{
			matchseqnum = node.text().toInt();
		}
		else if (node.tagName() == "tower_status_radiant")
		{
			radianttower = node.text().toInt();
		}
		else if (node.tagName() == "tower_status_dire")
		{
			diretower = node.text().toInt();
		}
		else if (node.tagName() == "barracks_status_radiant")
		{
			radiantbarracks = node.text().toInt();
		}
		else if (node.tagName() == "barracks_status_dire")
		{
			direbarracks = node.text().toInt();
		}
		else if (node.tagName() == "cluster")
		{
			cluster = node.text().toInt();
		}
		else if (node.tagName() == "first_blood_time")
		{
			firstbloodtime = node.text().toInt();
		}
		else if (node.tagName() == "lobby_type")
		{
			lobbytype = node.text().toInt();
		}
		else if (node.tagName() == "human_players")
		{
			humanplayer = node.text().toInt();
		}
		else if (node.tagName() == "leagueid")
		{
			leagueid = node.text().toInt();
		}
		else if (node.tagName() == "positive_votes")
		{
			positivevotes = node.text().toInt();
		}
		else if (node.tagName() == "negative_votes")
		{
			negativevotes = node.text().toInt();
		}
		else if (node.tagName() == "game_mode")
		{
			gamemode = node.text().toInt();
		}
		else if (node.tagName() == "engine")
		{
			engine = node.text().toInt();
		}
		else if (node.tagName() == "players")
		{
			for (auto playernode = node.firstChildElement("player"); !playernode.isNull(); playernode = playernode.nextSiblingElement("player"))
			{
				auto slotnode = playernode.firstChildElement("player_slot");
				int slot = slotnode.text().toInt();
				MatchDetail::PlayerDetail *player;
				if (slot < 128)
					player = &radiantplayers[slot];
				else
					player = &direplayers[slot - 128];

				for (auto anode = playernode.firstChildElement(); !anode.isNull(); anode = anode.nextSiblingElement())
				{
					if (anode.tagName() == "account_id")
					{
						player->accountid = anode.text().toInt();
						//if (player->accountid != 0)
						//{
						//	auto queue = QueueWaitFetchedPlayers::getInstancePtr();
						//	if (queue)
						//	{
						//		queue->push(player->accountid);
						//	}
						//}
					}
					else if (anode.tagName() == "hero_id")
						player->heroid = anode.text().toInt();
					else if (anode.tagName().contains("item_"))
					{
						int itemslot = anode.tagName().mid(5).toInt();
						player->item[itemslot].id = anode.text().toInt();
					}
					else if (anode.tagName() == "kills")
						player->kills = anode.text().toInt();
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
						for (auto abilitynode = anode.firstChildElement("ability"); !abilitynode.isNull(); abilitynode = abilitynode.nextSiblingElement("ability"))
						{
							int ability, level, time;
							ability = abilitynode.firstChildElement("ability").text().toInt();
							time = abilitynode.firstChildElement("time").text().toInt();
							level = abilitynode.firstChildElement("level").text().toInt();
							player->abilityupgrades[level - 1].ability = ability;
							player->abilityupgrades[level - 1].time = time;
						}
					}
					else if (anode.tagName() == "additional_units")
					{
						for (auto cnode = anode.firstChildElement(); !cnode.isNull(); cnode = cnode.nextSiblingElement())
						{
							if (cnode.tagName() == "unitname")
							{
								player->unitname = cnode.text();
							}
							else if (anode.tagName().contains("item_"))
							{
								int itemslot = anode.tagName().mid(5).toInt();
								player->uitem[itemslot].id = anode.text().toInt();
							}
						}
					}
					else
					{
						QString tag = anode.tagName();
					}
				}
			}
		}
		else if (node.tagName() == "picks_bans")
		{
			for (auto pbnode = node.firstChildElement("pick_ban"); !pbnode.isNull(); pbnode = pbnode.nextSiblingElement("pick_ban"))
			{
				int order = pbnode.firstChildElement("order").text().toInt();
				pickbanlist[order].ispick = pbnode.firstChildElement("is_pick").text() == "true";
				pickbanlist[order].heroid = pbnode.firstChildElement("hero_id").text().toInt();
				pickbanlist[order].team = pbnode.firstChildElement("team").text().toInt();
			}
		}
	}
}

void MatchDetail::handleData(bool)
{
	radiantgpm = radiantxpm = radiantherodamage = radianttowerdamage = radiantherohealing = 0;
	for (int i = 0; i < 5; ++i)
	{
		radiantgpm += radiantplayers[i].goldpermin;
		radiantxpm += radiantplayers[i].xppermin;
		radiantherodamage += radiantplayers[i].herodamage;
		radianttowerdamage += radiantplayers[i].towerdamage;
		radiantherohealing += radiantplayers[i].herohealing;
	}

	diregpm = direxpm = direherodamage = diretowerdamage = direherohealing = 0;
	for (int i = 0; i < 5; ++i)
	{
		diregpm += direplayers[i].goldpermin;
		direxpm += direplayers[i].xppermin;
		direherodamage += direplayers[i].herodamage;
		diretowerdamage += direplayers[i].towerdamage;
		direherohealing += direplayers[i].herohealing;
	}
}
