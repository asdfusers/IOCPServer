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


void CLobby::insertUserPool(CConnection User)
{
	LobbyClientPool.insert(std::pair<SOCKET, CConnection>(User.getSocket(), User));
}

void CLobby::deleteUserPool(CConnection User)
{
	LobbyClientPool.erase(User.getSocket());
}

CConnection CLobby::findUserPool(SOCKET socket)
{
	std::map<SOCKET, CConnection>::iterator itr;
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
