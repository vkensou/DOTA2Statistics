#pragma once

#include "singleton.h"

#include <map>

class AbilitiesList
	:public Singleton<AbilitiesList>
{
public:
	bool load();
	char getIndexByID(int id);

private:
	std::map<int, int> m_abilities;
};