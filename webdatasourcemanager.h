#pragma once

#include "singleton.h"
#include <QHash>
#include <QString>

class IWebDataSource;

class WebDataSourceManager
	:public Singleton < WebDataSourceManager >
{
public:
	enum WebDataSource{ DOTAMAX, DOTABUFF };

	WebDataSourceManager();
	~WebDataSourceManager();

	void setCurrentSource(WebDataSource source);

	IWebDataSource * getWebDataSource(WebDataSource source);
	IWebDataSource * getWebDataSourceCurrent();
	WebDataSource getWebDataSourceEnumCurrent();

private:
	QHash<WebDataSource, IWebDataSource *> m_list;
	WebDataSource m_currentsourceenum;
	IWebDataSource *m_currentsource;
};