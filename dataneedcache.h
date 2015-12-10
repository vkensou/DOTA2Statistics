#pragma once

#include "utility.h"

template<class T>
class DataNeedCache
{
public:
	~DataNeedCache()
	{
		pointerContainerDeleteAndClear(m_list);
	}

	void load(bool force_download = false)
	{
		if (!force_download)
		{
			if (!m_list.empty())
				return;

			if (loadFromDataBase())
				return;
		}
		pointerContainerDeleteAndClear(m_list);
		download();
		handleData(force_download);
		save();
	}

private:
	virtual bool loadFromDataBase() = 0;
	virtual void download() = 0;
	virtual void save() = 0;
	virtual void handleData(bool){};

protected:
	T m_list;
};
