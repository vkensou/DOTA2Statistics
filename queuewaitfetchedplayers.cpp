#include "queuewaitfetchedplayers.h"
#include "databasemanager.h"


QueueWaitFetchedPlayers::QueueWaitFetchedPlayers()
{

}

void QueueWaitFetchedPlayers::push(int playerid)
{
	if (m_queue.size() >= MAX_SIZE)
		return;

	if (DataBaseManager::getInstance().isPlayerSaved(playerid))
		return;

	m_queue.push(playerid);
	DataBaseManager::getInstance().insertPlayerInfo(playerid);
}

int QueueWaitFetchedPlayers::pop()
{
	if (m_queue.empty())
		return 0;
	else
	{
		int top = m_queue.front();
		m_queue.pop();
		return top;
	}
}

bool QueueWaitFetchedPlayers::isFull()
{
	return m_queue.size() == MAX_SIZE;
}

bool QueueWaitFetchedPlayers::isEmpty()
{
	return m_queue.empty();
}

int QueueWaitFetchedPlayers::getSize()
{
	return m_queue.size();
}
