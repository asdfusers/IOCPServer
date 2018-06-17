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
	virtual void insertUserPool(Socket* User);
	virtual void deleteUserPool(Socket* User);
	virtual Socket* findUserPool(SOCKET socket);
public:
	CLobby();
	~CLobby();

private:
	static CLobby*  m_pInst;
	std::map<SOCKET, Socket*> LobbyClientPool;

};

