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
	CMessageQue& getRecvQue() { return recvQue; }
	CMessageQue& getSendQue() { return sendQue; }

	void closeConnection();
	void onReceive(DWORD bytesTransferred);

private:
	CMessageQue recvQue;
	CMessageQue sendQue;


public:
	WSABUF wsaBuf;
	char buffer[PACKETBUFFERSIZE + 1];

	SOCKET m_socket;
	OVERLAPPED overlapped;
	IO_OPERATION ioType;
	int recvBytes;
	int sendBytes;

	char ID[10];
	int iLevel;
	int iRoomNum;
	int iTeam;
	Status eStatus;
	
	CS::CriticalSection Cric;

};

