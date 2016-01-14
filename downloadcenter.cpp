#include "downloadcenter.h"
#include <QMutexLocker>

DownloadCenter::DownloadCenter()
{
	m_time.start();
}

QString DownloadCenter::download(const QUrl &url, QNetworkReply::NetworkError &error, int extratime /*= 0*/, int retry /*= 0*/)
{
	error = QNetworkReply::NoError;
	QNetworkRequest request;
	QSslConfiguration config;

	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1SslV3);

	request.setSslConfiguration(config);
	request.setUrl(url);

	QNetworkAccessManager manager;
	qDebug() << url;
	int havetryed = 0;
	while (true)
	{
		while (m_time.elapsed() < m_lastdown + 0 + extratime)
		{
			QThread::msleep(50);
		}

		QNetworkReply *reply = manager.get(request);

		QEventLoop eventLoop;
		QObject::connect(reply, (void(QNetworkReply::*)(QNetworkReply::NetworkError))&QNetworkReply::error, [&eventLoop, &error](QNetworkReply::NetworkError replyerror)
		{
			error = replyerror;
			eventLoop.quit();
		});
		QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
		eventLoop.exec();       //block until finish

		m_lastdown = m_time.elapsed();
		if (reply->error())
		{
			QMutexLocker locker(&m_mutex);
			havetryed++;
			qDebug() << reply->error();
			if (havetryed > retry)
				break;
			qDebug() << "retry download " << havetryed + 1;
		}
		else
			return reply->readAll();
	}
	if (error != QNetworkReply::NoError)
	{
		m_numoferror++;
		qDebug() << error;
	}
	return "";
}
