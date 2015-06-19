#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>

const QString key = "387B6D180AD105C6CD289B0556C7A846";

QString downloadWebPage(const QUrl &url)
{
    QNetworkRequest request;
    QSslConfiguration config;

    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);

    request.setSslConfiguration(config);
    request.setUrl(url);

    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(request);

    QEventLoop eventLoop;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish
    return reply->readAll();
}

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
