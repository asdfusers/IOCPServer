#pragma once
#include "AcceptThread.h"
#include "WorkerThread.h"

class CIOCPserver
{
public:
	CIOCPserver();
	~CIOCPserver();

	bool sendMessage(CPacket packet);
	bool packetParsing(CPacket packet);


	bool Init(int PORT);
	void UnInit();
	void Update();
	void AcceptThreadInit();

private:
	int retval;
	SOCKET m_ListenSocket;
	HANDLE hcp, hThread;

private:
	CAcceptThread acceptThread;
	WorkerThread workerThread[WorkerThreadCount];
};

