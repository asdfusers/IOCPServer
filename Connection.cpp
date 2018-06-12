#include "stdafx.h"
#include "Connection.h"


CConnection::CConnection() : m_socket(INVALID_SOCKET), recvBytes(0), sendBytes(0)
{
}


CConnection::~CConnection()
{
	
}

void CConnection::close()
{
	if (m_socket != NULL)
		closesocket(m_socket);
	WSACleanup();
}
