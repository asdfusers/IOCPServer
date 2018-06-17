#include "stdafx.h"
#include "UserManager.h"


CUserManager* CUserManager::m_pInst = NULL;

CUserManager * CUserManager::getInst()
{
	if (m_pInst == NULL)
		m_pInst = new CUserManager();

	return m_pInst;
}

void CUserManager::releaseInst()
{
	if (m_pInst != NULL)
		delete m_pInst;

	m_pInst = 0;
}

void CUserManager::insertUser(Socket* client)
{
	clientPool.insert(std::pair<SOCKET, Socket*>(client->getSocket() , client));
}

void CUserManager::deleteUser(Socket client)
{
	clientPool.erase(client.getSocket());
}

std::map<SOCKET, Socket*>::iterator CUserManager::findUser(SOCKET socket)
{
	itr = clientPool.begin();
	while (itr != clientPool.end())
	{
		if (itr->first == socket)
			break;
		else
			itr++;
	}
	return itr;
}



