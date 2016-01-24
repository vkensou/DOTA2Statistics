#pragma once

#include <QThread>

class NeedDownload
	:public QThread
{
	Q_OBJECT
public:

signals :
	void threadstop();
};