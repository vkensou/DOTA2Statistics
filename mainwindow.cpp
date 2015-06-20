#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include "heroesrateandused.h"
#include "heroitems.h"
#include "herolist.h"

const QString key = "387B6D180AD105C6CD289B0556C7A846";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    HeroList herolist;
    herolist.load();

    HeroesRateAndUsed hr;
    hr.load("rateandused.xml");

#if 0
    QString name = "leshrac";
    QString chinese_name = herolist.getChineseNameByName(name);
#else
    QString chinese_name = "钢背兽";
    QString name = herolist.getNameByChineseName(chinese_name);
#endif

    HeroItems hero(name);
    hero.load();
    hero.calcX2(hr.getUsed(chinese_name), hr.getRate(chinese_name));
}

MainWindow::~MainWindow()
{
    delete ui;
}
