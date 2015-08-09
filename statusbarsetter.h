#pragma once

#include <QObject>
#include "singleton.h"

class QString;

class StatusBarSeter
	:public QObject, public Singleton<StatusBarSeter>
	//:public 
{
	Q_OBJECT
public:
	static void setStatusBar(const QString &text)
	{
		getInstance().setStatusBar_impl(text);
	}

signals:
	void setStatusBar_impl(const QString &text);
};