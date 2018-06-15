#pragma once
#include "stdafx.h"
#include "Connection.h"

class CUserManager
{
public:
	static CUserManager* getInst();
	static void releaseInst();

private:
	static  CUserManager* m_pInst;


public:
	void insertUser(CConnection& client);
	void deleteUser(CConnection client);
	std::map<SOCKET, CConnection>::iterator findUser(SOCKET socket);

public:
	std::map<SOCKET, CConnection> clientPool;
	std::map<SOCKET, CConnection>::iterator itr;
};



