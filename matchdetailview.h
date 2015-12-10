#pragma once

#include "idataview.h"
#include <QUrl>

namespace Ui
{
	class View_MatchDetail;
}

class QStyledItemDelegate;

class MatchDetailView
	:public IDataView
{
	Q_OBJECT
public:
	MatchDetailView();
	~MatchDetailView();

	void showMatchDetail(int matchid);

	virtual int getType() override;

	virtual const QString & getViewName() override;

	virtual void on_DataSource_Changed() override;
public slots:
	void on_ShowMatchDetail_clicked();

private:
	QUrl getMatchDetailURL(int matchid);
	void parseMatchDetailData(QString &data);
	void showData();

private:
	Ui::View_MatchDetail *ui;
	QStyledItemDelegate *itemimagedelegate;
	struct MatchDetail
	{
		int victoryparty{ 0 };
		int duration{ 0 };
		long long int starttime{ 0 };
		int matchid{ 0 };
		int matchseqnum{ 0 };
		int radianttower{ 0 };
		int diretower{ 0 };
		int radiantbarracks{ 0 };
		int direbarracks{ 0 };
		int cluster{ 0 };
		int firstbloodtime{ 0 };
		int lobbytype{ 0 };
		int humanplaer{ 0 };
		int leagueid{ 0 };
		int positivevotes{ 0 };
		int negativevotes{ 0 };
		int gamemode{ 0 };
		int engine{ 0 };
		struct PlayerDetail
		{
			int accountid{ 0 };
			int heroid{ 0 };
			int item0{ 0 }, item1{ 0 }, item2{ 0 }, item3{ 0 }, item4{ 0 }, item5{ 0 };
			int kilss{ 0 }, deaths{ 0 }, assists{ 0 };
			int level{ 0 };
			int gold{ 0 };
			int lasthits{ 0 }, denies{ 0 };
			int goldpermin{ 0 }, xppermin{ 0 };
			int goldspent{ 0 };
			int herodamage{ 0 }, towerdamage{ 0 };
			int herohealing{ 0 };
			int leaverstatus{ 0 };
			struct AbilityUpgradesDetail
			{
				int ability{ 0 };
				int time{ 0 };
				int level{ 0 };
			}abilityupgrades[25];
		};
		PlayerDetail radiantplayers[5];
		PlayerDetail direplayers[5];
	}m_match;
};

