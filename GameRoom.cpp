#include "stdafx.h"
#include "GameRoom.h"


CGameRoom::CGameRoom()
{
	m_Stage = new CStage;
}


CGameRoom::~CGameRoom()
{
}

void CGameRoom::insertUserPool(Socket* User)
{
	User->iRoomNum = iRoomNumber;
	PlayerPool.insert(std::pair<SOCKET, Socket*>(User->getSocket(), User));
}

void CGameRoom::deleteUserPool(Socket* User)
{
	PlayerPool.erase(User->getSocket());
}

Socket* CGameRoom::findUserPool(SOCKET socket)
{
	std::map<SOCKET, Socket*>::iterator itr;
	itr = PlayerPool.begin();

	while (itr != PlayerPool.end())
	{
		if (itr->first == socket)
			return itr->second;

		else
			itr++;
	}

}

void CGameRoom::GameStart()
{
	m_Stage->Init();
}
