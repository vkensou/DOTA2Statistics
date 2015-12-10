#pragma once

#include "dataneedcache.h"

class MatchDetail
	:public DataNeedCache
{


	virtual bool loaded() override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	virtual bool loadFromDataBase() override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	virtual void download() override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	virtual void save() override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	virtual void clear() override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

};