#pragma once

#include "dataneedcache.h"
#include <QHash>

template<class T>
class HashDataNeedCache
	:public DataNeedCache
{
public:
	~HashDataNeedCache()
	{
		clear();
	}

private:
	virtual bool loaded() final
	{
		return !m_list.empty();
	}
	virtual void clear() final
	{
		pointerContainerDeleteAndClear(m_list);
	}

protected:
	QHash<QString, T*> m_list;
};