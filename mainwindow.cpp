#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include "HeroesUsedAndRate.h"
#include "heroitems.h"
#include "herolist.h"
#include "dataconfig.h"
#include <QValidator>
#include "heroitemsmanager.h"

const QString key = "387B6D180AD105C6CD289B0556C7A846";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DataConfig::loadCurrent("datastatistics.ini");

    QStringList header;
    header << "物品" << "X2";
    ui->table_items->setHorizontalHeaderLabels(header);

    updateConfigPanel();

    datamanager.opendb();

    m_herolist.load();
}

MainWindow::~MainWindow()
{
    datamanager.closedb();
    DataConfig::saveCurrent("datastatistics.ini");
    delete ui;
}

void MainWindow::showItemsX2(const HeroItems &items)
{
    ui->table_items->clear();
    ui->table_items->setRowCount(items.list.count());
    int i = 0;
    auto func = [this, &i](const HeroItems::ItemRateAndUsed &item)
    {
        ui->table_items->setItem(i, 0, new QTableWidgetItem(item.name));
        QTableWidgetItem* wgt_x2 = new QTableWidgetItem();
        wgt_x2->setData(Qt::DisplayRole, (int)item.x2);
        wgt_x2->setTextAlignment(Qt::AlignRight);
        ui->table_items->setItem(i, 1, wgt_x2);
        ++i;
    };
    std::for_each(items.list.begin(), items.list.end(), func);

    ui->table_items->sortByColumn(1, Qt::DescendingOrder);
}

void MainWindow::updateConfigPanel()
{
    DataConfig &config = DataConfig::getCurrentConfig();

    ui->cbb_matchtype->setCurrentIndex((int)config.matchtype);
    ui->cbb_skill->setCurrentIndex((int)config.skill);
    ui->cbb_time->setCurrentIndex((int)config.time);
    ui->cbb_server->setCurrentIndex((int)config.server);
}

void MainWindow::on_cbb_time_currentIndexChanged(int index)
{
    DataConfig::getCurrentConfig().time = (DataConfig::Time)index;
}

void MainWindow::on_cbb_server_currentIndexChanged(int index)
{
    DataConfig::getCurrentConfig().server = (DataConfig::Server)index;
}

void MainWindow::on_cbb_skill_currentIndexChanged(int index)
{
    DataConfig::getCurrentConfig().skill = (DataConfig::Skill)index;
}

void MainWindow::on_cbb_matchtype_currentIndexChanged(int index)
{
    DataConfig::getCurrentConfig().matchtype = (DataConfig::MatchType)index;
}

void MainWindow::on_btn_calc_clicked()
{
    if(ui->edt_heroname->text().isEmpty())
        return;

    auto chinese_name = ui->edt_heroname->text();
    auto name = m_herolist.getNameByChineseName(chinese_name);
    if(name.isEmpty())
        return;

    bool force_download = ui->ckb_force_download->isChecked();

    HeroesUsedAndRate &hru = m_hrumanager.getHeroesUsedAndRate();

    HeroItems &hero = heroitemsmanager.getHeroItems(name);
    hero.load(force_download);
    hero.calcX2(hru.getUsed(chinese_name), hru.getRate(chinese_name));

    showItemsX2(hero);
}
