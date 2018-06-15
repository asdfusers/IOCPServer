#pragma once
#include "stdafx.h"
#include "Room.h"
class CLobby : public CRoom
{

public:
	static CLobby* getInst()
	{
		if (m_pInst == NULL)
			m_pInst = new CLobby;

		return m_pInst;
	}
	static void releaseInst();
	virtual void insertUserPool(CConnection User);
	virtual void deleteUserPool(CConnection User);
	virtual CConnection findUserPool(SOCKET socket);
public:
	CLobby();
	~CLobby();

private:
	static CLobby*  m_pInst;
	std::map<SOCKET, CConnection> LobbyClientPool;

};

