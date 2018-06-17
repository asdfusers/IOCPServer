#pragma once
#include "stdafx.h"
#include "Socket.h"

class CUserManager
{
public:
	static CUserManager* getInst();
	static void releaseInst();

private:
	static  CUserManager* m_pInst;


public:
	void insertUser(Socket* client);
	void deleteUser(Socket client);
	std::map<SOCKET, Socket*>::iterator findUser(SOCKET socket);

public:
	std::map<SOCKET, Socket*> clientPool;
	std::map<SOCKET, Socket*>::iterator itr;
};



