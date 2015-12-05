#pragma execution_character_set("utf-8")

#include "View_HeroItems.h"

#include "ui_view_heroitems.h"
#include "webdatasourcemanager.h"
#include "iwebdatasource.h"
#include "dataconfig.h"
#include "heroesusedandrate.h"
#include "heroitemsmanager.h"
#include "heroitems.h"
#include "herolist.h"
#include <QMessageBox>
#include "statusbarsetter.h"
#include <QCompleter>

View_HeroItems::View_HeroItems(QWidget *parent /*= nullptr*/)
	: IDataView(parent)
	, m_table_sortorder(true)
	, ui(new Ui::View_HeroItems)
	, m_completer(nullptr)
{
	ui->setupUi(this);

    connect(ui->table_items->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(on_table_sort(int)));

	setTableWidgetHead();
	setHeroNmaeCompleter();
	initConfigPanel();
}

View_HeroItems::~View_HeroItems()
{
	delete ui;
	delete m_completer;
}

void View_HeroItems::initConfigPanel()
{
	ui->cbb_time->clear();
	ui->cbb_server->clear();
	ui->cbb_matchtype->clear();
	ui->cbb_skill->clear();

	auto source = WebDataSourceManager::getInstance().getWebDataSourceCurrent();

	ui->cbb_time->setEnabled(source->isSupportSetTime());
	ui->cbb_server->setEnabled(source->isSupportSetServer());
	ui->cbb_matchtype->setEnabled(source->isSupportSetMatchType());
	ui->cbb_skill->setEnabled(source->isSupportSetSkill());

	ui->cbb_time->addItems(source->getTimeSetterTextList());
	ui->cbb_server->addItems(source->getServerSetterTextList());
	ui->cbb_matchtype->addItems(source->getMatchTypeSetterTextList());
	ui->cbb_skill->addItems(source->getSkillSetterTextList());

	updateConfigPanel();
}

void View_HeroItems::setHeroNmaeCompleter()
{
	QStringList namelist;

	auto &herolist = HeroList::getInstance().getHeroList();
	for each (auto & hero in herolist)
	{
		for each (auto & alias in hero.aliases)
		{
			namelist << alias;
		}
	}

	m_completer = new QCompleter(namelist);
	m_completer->setCaseSensitivity(Qt::CaseSensitive);
	m_completer->setCompletionMode(QCompleter::PopupCompletion);
	ui->edt_heroname->setCompleter(m_completer);
}

void View_HeroItems::updateConfig()
{
	DataConfig &config = DataConfig::getCurrentConfig();

	config.time = ui->cbb_time->currentIndex();
	config.server = ui->cbb_server->currentIndex();
	config.skill = ui->cbb_skill->currentIndex();
	config.matchtype = ui->cbb_matchtype->currentIndex();
}

void View_HeroItems::on_DataSource_Changed()
{
	initConfigPanel();
	updateConfigPanel();
}

int View_HeroItems::getType()
{
	return 0;
}

const QString & View_HeroItems::getViewName()
{
	static QString name = "英雄装备重要度";
	return name;
}

void View_HeroItems::updateConfigPanel()
{
	DataConfig &config = DataConfig::getCurrentConfig();

	ui->cbb_matchtype->setCurrentIndex((int)config.matchtype);
	ui->cbb_skill->setCurrentIndex((int)config.skill);
	ui->cbb_time->setCurrentIndex((int)config.time);
	ui->cbb_server->setCurrentIndex((int)config.server);
}

void View_HeroItems::on_btn_calc_clicked()
{
	if (ui->edt_heroname->text().isEmpty())
		return;
	updateConfig();

	auto alias = ui->edt_heroname->text();
	alias = alias.toLower();
	HeroList &herolist = HeroList::getInstance();
	auto name = herolist.getNameByAlias(alias);
	if (name.isEmpty())
	{
		QMessageBox::warning(NULL, windowTitle(), "没有这个英雄");
		return;
	}

	bool force_download = ui->ckb_force_download->isChecked();

	HeroItemsManager &heroitemsmanager = HeroItemsManager::getInstance();
	HeroItems &hero = heroitemsmanager.getHeroItems(name);
	hero.load(force_download);

	showItemsX2(hero);

	StatusBarSeter::getInstance().setStatusBar("Ready");
}

void View_HeroItems::showItemsX2(const HeroItems &items)
{
	StatusBarSeter::getInstance().setStatusBar("Table updating...");

    ui->table_items->clear();
    setTableWidgetHead();
    ui->table_items->setRowCount(items.getItemsCount());
    int i = 0;
	std::function<void(const ItemRateAndUsed *)> func = [this, &i](const ItemRateAndUsed * item)
    {
        ui->table_items->setItem(i, 0, new QTableWidgetItem(item->name));
        QTableWidgetItem* wgt_x2 = new QTableWidgetItem();
		wgt_x2->setData(Qt::DisplayRole, (int)item->x2);
        wgt_x2->setTextAlignment(Qt::AlignRight);
        ui->table_items->setItem(i, 1, wgt_x2);
        ++i;
    };
    items.for_each_items(func);

    m_table_sortorder = true;
    ui->table_items->sortByColumn(1, Qt::DescendingOrder);

	StatusBarSeter::getInstance().setStatusBar("Table update complete");
}

void View_HeroItems::setTableWidgetHead()
{
	QStringList header;
	header << "物品" << "X2";
	ui->table_items->setHorizontalHeaderLabels(header);
}

void View_HeroItems::on_table_sort(int column)
{
    if(column == 1)
    {
        m_table_sortorder = !m_table_sortorder;
        if(m_table_sortorder)
            ui->table_items->sortByColumn(1, Qt::DescendingOrder);
        else
            ui->table_items->sortByColumn(1, Qt::AscendingOrder);
    }
}
