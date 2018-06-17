#include "stdafx.h"
#include "Lobby.h"

CLobby* CLobby::m_pInst = NULL;

void CLobby::releaseInst()
{
	if (m_pInst != NULL)
	{
		delete m_pInst;
		m_pInst = NULL;
	}
}


void CLobby::insertUserPool(Socket* User)
{
	LobbyClientPool.insert(std::pair<SOCKET, Socket*>(User->getSocket(), User));
}

void CLobby::deleteUserPool(Socket* User)
{
	LobbyClientPool.erase(User->getSocket());
}

Socket* CLobby::findUserPool(SOCKET socket)
{
	std::map<SOCKET, Socket*>::iterator itr;
	itr = LobbyClientPool.begin();

	while (itr != LobbyClientPool.end())
	{
		if (itr->first == socket)
			return itr->second;

		else
			itr++;
	}

}

CLobby::CLobby()
{
}


CLobby::~CLobby()
{
}
