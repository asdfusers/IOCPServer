#include "stdafx.h"
#include "Connection.h"


CConnection::CConnection() 
{
	
}


CConnection::~CConnection()
{
	
}

void CConnection::insertList(Socket* _socket)
{
	socketList.push_back(_socket);
}

void CConnection::removeList(Socket* _socket)
{
	itr = socketList.begin();
	while (itr != socketList.end())
	{
		if ((*itr)->m_socket == _socket->m_socket)
		{
			itr = socketList.erase(itr++);
			break;
		}
		else
			itr++;
	}
}

