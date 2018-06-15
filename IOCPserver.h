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

	void onLoginPacket1Req(CPacket & packet);
	void onPSelectLobbyOption(CPacket & packet);
	void onPPlayerEnterRoom(CPacket & packet);
	void onPBroadCastEnterRoom(CPacket & packet);
	void onPReadyReq(CPacket & packet);
	void onPReadyResult(CPacket & packet);
	void onPGameStartReady(CPacket & packet);
	void onPGameStart(CPacket & packet);
	void onPGameInputKey(CPacket & packet);

	bool playerPositionSetting(std::string cInputKey, CPosition pos, CPacket & packet);
	void DeleteUserPool(SOCKET socket);
	void ChoiceLobbyOption(int iNum, SOCKET socket);
	std::string ViewUserInformation(CConnection User);
	std::string VIewUserStatuInformation(CConnection User);

private:
	int retval;
	SOCKET m_ListenSocket;
	HANDLE hcp, hThread;
	CMessageQue sendMessageQue;

	CS::CriticalSection cs;

private:
	CAcceptThread acceptThread;
	WorkerThread workerThread[WorkerThreadCount];
};

