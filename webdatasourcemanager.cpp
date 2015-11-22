#include "webdatasourcemanager.h"
#include "webdatasourcedotamax.h"
#include "webdatasourcedotabuff.h"
#include "utility.h"

WebDataSourceManager::WebDataSourceManager()
{
	m_list.insert(DOTAMAX, new WebDataSource_DotaMax);
	m_list.insert(DOTABUFF, new WebDataSource_DotaBuff);
	setCurrentSource(DOTAMAX);
}

WebDataSourceManager::~WebDataSourceManager()
{
	pointerContainerDeleteAndClear(m_list);
}

void WebDataSourceManager::setCurrentSource(WebDataSource source)
{
	m_currentsource = m_list[source];
	m_currentsourceenum = source;
}

IWebDataSource * WebDataSourceManager::getWebDataSource(WebDataSource source)
{
	return m_list[source];
}

IWebDataSource * WebDataSourceManager::getWebDataSourceCurrent()
{
	return m_currentsource;
}

WebDataSourceManager::WebDataSource WebDataSourceManager::getWebDataSourceEnumCurrent()
{
	return m_currentsourceenum;
}
