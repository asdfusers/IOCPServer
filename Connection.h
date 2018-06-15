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
	OVERLAPPED getOverlapped() { return overlapped; }
	int getRecvBytes() { return recvBytes; }
	int getSendBytes() { return sendBytes; }
	WSABUF getWsaBuf() { return wsaBuf; }
	DWORD getFlags() { return flags; }


	WSABUF wsaBuf;
	char buffer[PACKETBUFFERSIZE + 1];
	void SetRecvBytes(int _recv) { recvBytes = _recv; }
	void SetSendBytes(int _send) { sendBytes = _send; }
	void SetFlags(DWORD _flags) { flags = _flags; }
	
	char* getID() { return ID; }
	int getLevel() { return iLevel; }
	int getRoomidx() { return iRoomNum; }
	int getPlayerTeam() { return iTeam; }
	Status getStatus() { return eStatus; }




	void setIteam(int _iTeam) { iTeam = _iTeam; }
	void setID(char _ID[10]) { strcpy(ID, _ID); }
	void setiLevel(int _ilv) { iLevel = _ilv; }
	void setStatus(Status status) { eStatus = status; }
	void setRoomNum(int _room) { iRoomNum = _room; }

	OVERLAPPED overlapped;

	DWORD flags;
	int recvBytes;
	int sendBytes;
	SOCKET m_socket;

private:

	char ID[10];
	int iLevel;
	int iRoomNum;
	int iTeam;
	Status eStatus;
};

