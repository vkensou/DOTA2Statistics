#pragma execution_character_set("utf-8")

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include "HeroesUsedAndRate.h"
#include "heroitems.h"
#include "herolist.h"
#include "dataconfig.h"
#include <QValidator>
#include "heroitemsmanager.h"
#include <QMessageBox>
#include "version.h"
#include "statusbarsetter.h"
#include "iwebdatasource.h"

const QString key = "387B6D180AD105C6CD289B0556C7A846";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_table_sortorder(true)
{
    ui->setupUi(this);

	m_webdatasourcemanager.setCurrentSource(WebDataSourceManager::DOTAMAX);
	
	initStatusBar();
	initConfigPanel();

	setWindowTitle("DOTA2统计学 V"PRODUCT_VERSION_STR);

    connect(ui->table_items->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(on_table_sort(int)));

    DataConfig::loadCurrent("datastatistics.ini");

    setTableWidgetHead();

    updateConfigPanel();

    m_databasemanager.opendb();

    m_herolist.load();
}

MainWindow::~MainWindow()
{
    m_databasemanager.closedb();
    DataConfig::saveCurrent("datastatistics.ini");
    delete ui;
	delete m_statusbarsetter;
}

void MainWindow::initStatusBar()
{
	m_statusbarsetter = new StatusBarSeter;
	connect(StatusBarSeter::getInstancePtr(), SIGNAL(setStatusBar_impl(QString)), this, SLOT(setStatusBarText(QString)));
	setStatusBarText("Ready");
}

void MainWindow::initConfigPanel()
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
	ui->cbb_server->addItems(source->getServerSetterText());
	ui->cbb_matchtype->addItems(source->getMatchTypeSetterText());
	ui->cbb_skill->addItems(source->getSkillSetterText());

	updateConfigPanel();
}

void MainWindow::showItemsX2(const HeroItems &items)
{
	setStatusBarText("Table updating...");

    ui->table_items->clear();
    setTableWidgetHead();
    ui->table_items->setRowCount(items.getItemsCount());
    int i = 0;
    auto func = [this, &i](const ItemRateAndUsed * item)
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

	setStatusBarText("Table update complete");
}

void MainWindow::updateConfigPanel()
{
    DataConfig &config = DataConfig::getCurrentConfig();

    ui->cbb_matchtype->setCurrentIndex((int)config.matchtype);
    ui->cbb_skill->setCurrentIndex((int)config.skill);
    ui->cbb_time->setCurrentIndex((int)config.time);
    ui->cbb_server->setCurrentIndex((int)config.server);
}

void MainWindow::setTableWidgetHead()
{
    QStringList header;
    header << "物品" << "X2";
    ui->table_items->setHorizontalHeaderLabels(header);
}

void MainWindow::setStatusBarText(const QString &text)
{
	ui->statusBar->showMessage(text);
	//ui->statusBar->messageChanged(text);
}

void MainWindow::on_cbb_time_currentIndexChanged(int index)
{
    DataConfig::getCurrentConfig().time = index;
}

void MainWindow::on_cbb_server_currentIndexChanged(int index)
{
    DataConfig::getCurrentConfig().server = index;
}

void MainWindow::on_cbb_skill_currentIndexChanged(int index)
{
    DataConfig::getCurrentConfig().skill = index;
}

void MainWindow::on_cbb_matchtype_currentIndexChanged(int index)
{
    DataConfig::getCurrentConfig().matchtype = index;
}

void MainWindow::on_btn_calc_clicked()
{
    if(ui->edt_heroname->text().isEmpty())
        return;

    auto chinese_name = ui->edt_heroname->text();
    auto name = m_herolist.getNameByChineseName(chinese_name);
    if(name.isEmpty())
	{
		QMessageBox::warning(NULL, windowTitle(), "没有这个英雄");
		return;
	}

    bool force_download = ui->ckb_force_download->isChecked();

    HeroItems &hero = m_heroitemsmanager.getHeroItems(name);
    hero.load(force_download);

    showItemsX2(hero);

	setStatusBarText("Ready");
}

void MainWindow::on_action_about_triggered()
{
    QString title = "关于";
	QString text = "版本："PRODUCT_VERSION_STR"\r\nDeveloped by vkensou\r\nPowered by QT";

    QMessageBox::about(NULL, title, text);
}

void MainWindow::on_table_sort(int column)
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
