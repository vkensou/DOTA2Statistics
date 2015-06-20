#include "utility.h"

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
