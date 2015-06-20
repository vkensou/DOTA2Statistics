#include "utility.h"
#include <QRegExp>

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

int sepNumStrtoInt(const QString &str)
{
    QString s = str;
    s.replace(",", "");

    return s.toInt();
}

float percentagetoFloat(const QString &str)
{
    static QRegExp rx("^([+-]?\\d+(\\.\\d+)?)%$");
    if(rx.indexIn(str) >= 0)
        return rx.cap(1).toFloat() /100;
    else
        return 0;
}
