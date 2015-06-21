#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include "heroesrateandused.h"
#include "heroitems.h"
#include "herolist.h"
#include "dataconfig.h"
#include <QValidator>

const QString key = "387B6D180AD105C6CD289B0556C7A846";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DataConfig::loadCurrent("datastatistics.ini");

    HeroList herolist;
    herolist.load();

    HeroesRateAndUsed hr;
    hr.load();

#if 0
    QString name = "leshrac";
    QString chinese_name = herolist.getChineseNameByName(name);
#else
    QString chinese_name = "美杜莎";
    QString name = herolist.getNameByChineseName(chinese_name);
#endif

    HeroItems hero(name);
    hero.load();
    hero.calcX2(hr.getUsed(chinese_name), hr.getRate(chinese_name));

    showItemsX2(hero);
//    auto p = hero.getX2()
}

MainWindow::~MainWindow()
{
    DataConfig::saveCurrent("datastatistics.ini");
    delete ui;
}

void MainWindow::on_btn_CalcX2_clicked()
{
    int hu = ui->spin_HeroUsed->value();
    double hr = ui->spin_HeroRate->value();
    int iu = ui->spin_ItemUsed->value();
    double ir = ui->spin_ItemRate->value();

    double x2 = HeroItems::getX2(hu, hr, iu, ir);
    ui->lbl_result->setText(QString::number(x2));
}

void MainWindow::showItemsX2(const HeroItems &items)
{
    ui->table_items->clear();
    ui->table_items->setRowCount(items.list.count());
    QStringList header;
    header << "物品" << "X2";
    ui->table_items->setHorizontalHeaderLabels(header);
    int i = 0;
    auto func = [this, &i](const HeroItems::ItemRateAndUsed &item)
    {
        ui->table_items->setItem(i, 0, new QTableWidgetItem(item.name));
        QTableWidgetItem* wgt_x2 = new QTableWidgetItem();
        wgt_x2->setData(Qt::DisplayRole, item.x2);
        ui->table_items->setItem(i, 1, wgt_x2);
        ++i;
    };
    std::for_each(items.list.begin(), items.list.end(), func);

    ui->table_items->sortByColumn(1, Qt::DescendingOrder);
}
