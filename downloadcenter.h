#pragma once

#include "singleton.h"
#include <QMutex>
#include <QtNetwork>
#include <QTime>

class DownloadCenter
	:public Singleton<DownloadCenter>
{
public:
	DownloadCenter();

	QString download(const QUrl &url, QNetworkReply::NetworkError &error, int extratime = 0, int retry = 0);
	int getNumOfError()
	{
		return m_numoferror;
	}
private:
	QMutex m_mutex;
	int m_lastdown = -1000;
	QTime m_time;
	int m_numoferror{ 0 };
};