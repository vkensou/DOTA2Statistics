#pragma once

#include "utility.h"
#include <functional>

template<class T1, class T2>
class DataManagerBase
{
public:
	~DataManagerBase()
	{
		pointerContainerDeleteAndClear(m_list);
	}

protected:
	T2 & getDatum(const QString &key, std::function<T2*()> &factory)
	{
		auto i = m_list.find(key);
		if (i == m_list.end())
		{
			i = m_list.insert(key, factory());
		}
		T2 * data = i.value();
		return *data;
	}

private:
	T1 m_list;

};