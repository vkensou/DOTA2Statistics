#pragma once

#include <QThread>
#include "singleton.h"

class MatchDetailSaveThread
	:public QThread, public Singleton<MatchDetailSaveThread>
{
public:
	int getSaved();
	int getCount();

	virtual void run() override;

private:
	int m_count{ 0 };
};