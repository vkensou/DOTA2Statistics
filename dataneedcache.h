#pragma once

class DataNeedCache
{
public:
	void load(bool force_download = false)
	{
		if (!force_download)
		{
			if (loaded())
				return;

			if (loadFromDataBase())
				return;
		}
		clear();
		download();
		handleData(force_download);
		save();
	}

	virtual void save() = 0;

private:
	virtual bool loaded() = 0;
	virtual bool loadFromDataBase() = 0;
	virtual void download() = 0;
	virtual void handleData(bool){};
	virtual void clear() = 0;
};
