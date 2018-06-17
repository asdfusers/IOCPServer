#pragma once
#include "stdafx.h"
#include "Packet.h"
#include "CMessageQue.h"
class Socket
{
public:
	Socket();
	~Socket();

public:
	SOCKET getSocket() { return m_socket; }
	char* getID() { return ID; }
	int getLevel() { return iLevel; }
	int getRoomidx() { return iRoomNum; }
	int getPlayerTeam() { return iTeam; }
	OVERLAPPED getOverlapped() { return overlapped; }
	Status getStatus() { return eStatus; }

	WSABUF wsaBuf;
	char buffer[PACKETBUFFERSIZE + 1];

	SOCKET m_socket;
	OVERLAPPED overlapped;
	DWORD flags;
	int recvBytes;
	int sendBytes;


	CMessageQue getRecvQue() { return recvQue; }
	CMessageQue getSendQue() { return sendQue; }
private:
	CMessageQue recvQue;
	CMessageQue sendQue;

public:

	char ID[10];
	int iLevel;
	int iRoomNum;
	int iTeam;
	Status eStatus;
};

