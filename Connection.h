#pragma once
#include "stdafx.h"
#include "Packet.h"

#define READ 0x00
#define WRITE 0x01

class CConnection
{
	
public:
	CConnection();
	~CConnection();



	void close();
	void SetSocket(SOCKET socket) { m_socket = socket; }


	




	SOCKET getSocket() { return m_socket; }
	WSAOVERLAPPED getOverlapped() { return overlapped; }
	int getRecvBytes() { return recvBytes; }
	int getSendBytes() { return sendBytes; }
	WSABUF getWsaBuf() { return wsaBuf; }

	WSABUF wsaBuf;

	SOCKET m_socket;
	WSAOVERLAPPED overlapped;
	char buffer[PACKETBUFFERSIZE];
	int recvBytes;
	int sendBytes;
private:
};

