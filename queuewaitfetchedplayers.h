#pragma once

#include <queue>

class QueueWaitFetchedPlayers
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