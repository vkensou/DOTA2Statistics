#include "webdatasourcemanager.h"
#include "webdatasourcedotamax.h"

WebDataSourceManager::WebDataSourceManager()
{
	m_list.insert(DOTAMAX, QSharedPointer<IWebDataSource>(new WebDataSource_DotaMax));
	setCurrentSource(DOTAMAX);
}

WebDataSourceManager::~WebDataSourceManager()
{

}

void WebDataSourceManager::setCurrentSource(WebDataSource source)
{
	m_currentsource = m_list[source].data();
	m_currentsourceenum = source;
}

IWebDataSource * WebDataSourceManager::getWebDataSource(WebDataSource source)
{
	return m_list[source].data();
}

IWebDataSource * WebDataSourceManager::getWebDataSourceCurrent()
{
	return m_currentsource;
}

WebDataSourceManager::WebDataSource WebDataSourceManager::getWebDataSourceEnumCurrent()
{
	return m_currentsourceenum;
}
