#pragma once
#include "stdafx.h"
#include "Packet.h"
#include "Socket.h"
#define READ 0x00
#define WRITE 0x01


class CConnection
{
public:
	CConnection();
	~CConnection();

	std::list<Socket*>& getSocketList() { return socketList; }
	void insertList(Socket* _socket);
	void removeList(Socket* _socket);



private:
	std::list<Socket*> socketList;
	std::list<Socket*>::iterator itr;

};

