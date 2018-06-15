#include "stdafx.h"
#include "GameRoom.h"


CGameRoom::CGameRoom()
{
	m_Stage = new CStage;
}


CGameRoom::~CGameRoom()
{
}

void CGameRoom::insertUserPool(CConnection& User)
{
	User.setRoomNum(iRoomNumber);
	PlayerPool.insert(std::pair<SOCKET, CConnection&>(User.getSocket(), User));
}

void CGameRoom::deleteUserPool(CConnection User)
{
	PlayerPool.erase(User.getSocket());
}

CConnection CGameRoom::findUserPool(SOCKET socket)
{
	std::map<SOCKET, CConnection&>::iterator itr;
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
