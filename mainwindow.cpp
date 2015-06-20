#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include "heroesrateandused.h"
#include "heroitems.h"

const QString key = "387B6D180AD105C6CD289B0556C7A846";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    HeroesRateAndUsed hr;
//    hr.download();
//    hr.save("rateandused.xml");

    hr.load("rateandused.xml");

    HeroItems hero("leshrac");
    hero.download();
    hero.save();

//    hero.load();
    hero.calcX2(97035, 0.5459);
}

MainWindow::~MainWindow()
{
    delete ui;
}
