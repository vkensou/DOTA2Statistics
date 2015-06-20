#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"


const QString key = "387B6D180AD105C6CD289B0556C7A846";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString result;
    QUrl url = "https://api.steampowered.com/IDOTA2Match_570/GetMatchHistory/V001/?key=" + key;
    result = downloadWebPage(url);
    result = result;
}

MainWindow::~MainWindow()
{
    delete ui;
}
