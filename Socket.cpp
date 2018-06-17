#include "stdafx.h"
#include "Socket.h"


Socket::Socket() : m_socket(INVALID_SOCKET), recvBytes(0), sendBytes(0), iLevel(0), eStatus(Connect), iRoomNum(0)
{
}


Socket::~Socket()
{
}
