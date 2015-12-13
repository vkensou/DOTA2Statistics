#pragma once

#include <queue>
#include "singleton.h"

class QueueWaitFetchedPlayers
	:public Singleton<QueueWaitFetchedPlayers>
{
public:
	QueueWaitFetchedPlayers();

	void push(int playerid);
	int pop();
	bool isFull();
	bool isEmpty();
	int getSize();

private:
	std::queue<int> m_queue;
	const int MAX_SIZE = 10;
};