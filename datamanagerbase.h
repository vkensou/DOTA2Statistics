#pragma once

#include "utility.h"
#include <functional>
#include <QHash>

template<class T1>
class DataManagerBase
{
public:
	~DataManagerBase()
	{
		pointerContainerDeleteAndClear(m_list);
	}

protected:
	T1 & getDatum(const QString &key, std::function<T1*()> &factory)
	{
		auto i = m_list.find(key);
		if (i == m_list.end())
		{
			i = m_list.insert(key, factory());
		}
		T1 *data = i.value();
		return *data;
	}

private:
	QHash<QString, T1 *> m_list;

};