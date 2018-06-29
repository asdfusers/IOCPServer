#include "stdafx.h"
#include "IOCPserver.h"
#include "UserManager.h"
#include "RoomManager.h"
#include "GameRoom.h"
#include "Lobby.h"
CIOCPserver::CIOCPserver() : m_ListenSocket(INVALID_SOCKET)
{

}


CIOCPserver::~CIOCPserver()
{
	if (m_ListenSocket != INVALID_SOCKET)
		closesocket(m_ListenSocket);

	WSACleanup();
}

bool CIOCPserver::sendMessage(CPacket packet)
{
	retval = send(packet.getSocketNumber(), packet.getPacketBuffer(), packet.getPacketSize(), 0);
	if (retval == SOCKET_ERROR)
		return false;
	return true;
}

bool CIOCPserver::packetParsing(CPacket packet)
{
	switch (packet.id())
	{
	case  P_LOGINPACKET_REQ:				onLoginPacket1Req(packet);			break;
	case  P_LOBBYOPTION_REQ:				onPSelectLobbyOption(packet);		break;
	case  P_ENTERROOM_REQ:					onPPlayerEnterRoom(packet);			break;
	case  P_BROADCAST_ENTER_ROOM_REQ:		onPBroadCastEnterRoom(packet);		break;
	case  P_READY_REQ:						onPReadyReq(packet);				break;
	case  P_READYRESULT_REQ:				onPReadyResult(packet);				break;
	case  P_GAMESTARTREADY_REQ:				onPGameStartReady(packet);			break;
	case  P_GAMESTART_REQ:					onPGameStart(packet);				break;
	case  P_GAMEINPUT_REQ:					onPGameInputKey(packet);			break;
	}
	return true;
}

bool CIOCPserver::Init(int PORT)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		std::cout << "WSAStartUp Error" << std::endl;

	m_ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_ListenSocket == INVALID_SOCKET)
		std::cout << "socket()" << std::endl;

	SOCKADDR_IN socketAddr;
	ZeroMemory(&socketAddr, sizeof(socketAddr));
	socketAddr.sin_port = htons(PORT);
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.S_un.S_addr = htonl(ADDR_ANY);

	int reuse = 1;
	setsockopt(m_ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));

	unsigned long arg = 1;
	ioctlsocket(m_ListenSocket, FIONBIO, &arg);

	retval = bind(m_ListenSocket, (SOCKADDR*)&socketAddr, sizeof(socketAddr));
	if (retval == INVALID_SOCKET)
		std::cout << "bind()" << std::endl;

	retval = listen(m_ListenSocket, SOMAXCONN);
	if (retval == INVALID_SOCKET)
		std::cout << "listen()" << std::endl;

	hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	
	if (hcp == NULL)
		std::cout << "CreateFail()" << std::endl;


	SYSTEM_INFO si;
	GetSystemInfo(&si);

	for (int i = 0; i < WorkerThreadCount; ++i)
	{
		workerThread[i].Set_IOCP_HANDLE(hcp);
		workerThread[i].begin();
	}
	XTrace(L"%d", hcp);
	AcceptThreadInit();
	return true;
	
}

void CIOCPserver::UnInit()
{
	acceptThread.join();
	for (int i = 0; i < WorkerThreadCount; ++i)
	{
		workerThread[i].join();
	}
}

void CIOCPserver::Update()
{	
	if (matchingQue.size() > 1)
	{
		MatchingCheck();
	}
	for (auto socket : CUserManager::getInst()->clientPool)
	{
		if (!socket.second->getRecvQue().messageQue.empty())
		{
			CCriticalSectionLock cs(socket.second->Cric);
			packetParsing(socket.second->getRecvQue().messageQue.front());
			socket.second->getRecvQue().messageQue.pop();
		}
	}

	if (!sendMessageQue.messageQue.empty())
	{
		while (!sendMessageQue.messageQue.empty())
		{
			sendMessage(sendMessageQue.messageQue.front());
			sendMessageQue.messageQue.pop();
		}
	}
}

void CIOCPserver::AcceptThreadInit()
{
	acceptThread.SetListenSocket(m_ListenSocket);
	acceptThread.Set_IOCP_Handle(hcp);
	acceptThread.begin();
}


void CIOCPserver::onLoginPacket1Req(CPacket & packet)
{
	{
		Login log;
		packet >> log;
		std::cout << "ID : " << log.ID << "\t PASSWORD : " << log.password << std::endl;
		wcscpy_s(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->ID, log.ID);

		CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iLevel = 1;
		CUserManager::getInst()->findUser(packet.getSocketNumber())->second->eStatus = Lobby;
		CLobby::getInst()->insertUserPool(CUserManager::getInst()->clientPool.find(packet.getSocketNumber())->second);
	}

	{
		CPacket sendPacket(P_LOGINPACKET_ACK);
		sendPacket.SetSocketNumber(packet.getSocketNumber());
		sendPacket << L"여기는 로비입니다. \n1. 방 입장하기 ";
		sendMessageQue.messageQue.push(sendPacket);
	}
}

void CIOCPserver::onPSelectLobbyOption(CPacket & packet)
{
	wchar_t iNumber[2];
	packet >> iNumber;
	printf("%d\n", iNumber);
	ChoiceLobbyOption(iNumber, packet.getSocketNumber());
	
	wchar_t cInput[5];
	CPosition pos;
	packet >> cInput >> pos;

	char* pStr;
	int strSize = WideCharToMultiByte(CP_ACP, 0, iNumber, -1, NULL, 0, NULL, NULL);
	pStr = new char[strSize];
	WideCharToMultiByte(CP_ACP, 0, iNumber, -1, pStr, strSize, 0, 0);

	std::string asdf;
	for (int i = 0; i < sizeof(pStr); i++)
	{
		asdf += pStr[i];
	}

	int num = atoi(asdf.c_str());
	switch (num)
	{
	case 1:
	{
		CPacket sendPacket(P_MATCHING_ACK);
		sendPacket.SetSocketNumber(packet.getSocketNumber());
		sendPacket << L"************************* 랜덤 매칭중 *************************";
		sendMessageQue.messageQue.push(sendPacket);
	}
	break;
	default:
	{
		CPacket sendPacket(P_LOGINPACKET_ACK);
		sendPacket.SetSocketNumber(packet.getSocketNumber());
		sendPacket << L"여기는 로비입니다. \n1. 방 입장하기 ";
		sendMessageQue.messageQue.push(sendPacket);
	}
	break;
	}
}

void CIOCPserver::onPPlayerEnterRoom(CPacket & packet)
{
	{
		int iInput;
		packet >> iInput;
		printf("%d\n", iInput);

		if (CRoomManager::getinst()->getRoomPool().empty())
		{
			CPacket sendPacket(P_LOGINPACKET_ACK);
			sendPacket.SetSocketNumber(packet.getSocketNumber());
			sendPacket << L"여기는 로비입니다. \n1. 방 입장하기\n";
			sendMessageQue.messageQue.push(sendPacket);
			return;
		}
		else
		{
			for (auto Room : CRoomManager::getinst()->getRoomPool())
			{
				if (Room.GetRoomNumber() == iInput)
				{
					if (CRoomManager::getinst()->findRoom(iInput)->getPool().size() >= 2)
					{
						CPacket sendPacket(P_LOGINPACKET_ACK);
						sendPacket.SetSocketNumber(packet.getSocketNumber());
						sendPacket << L"여기는 로비입니다. \n1. 방 입장하기\n";
						sendMessageQue.messageQue.push(sendPacket);
						return;
					}

					continue;
				}
				else
				{
					CPacket sendPacket(P_LOGINPACKET_ACK);
					sendPacket.SetSocketNumber(packet.getSocketNumber());
					sendPacket << L"여기는 로비입니다. \n1. 방 입장하기\n";
					sendMessageQue.messageQue.push(sendPacket);
					return;
				}
			}

			CUserManager::getInst()->findUser(packet.getSocketNumber())->second->eStatus = InRoom;
			CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum = iInput;
			CRoomManager::getinst()->findRoom(iInput)->insertUserPool(CUserManager::getInst()->findUser(packet.getSocketNumber())->second);
		}
	}

	{
		std::string str = ViewUserInformation(CUserManager::getInst()->findUser(packet.getSocketNumber())->second);

		CPacket sendPacket(P_ENTERROOM_ACK);
		sendPacket.SetSocketNumber(packet.getSocketNumber());
		sendPacket << str;
		sendMessageQue.messageQue.push(sendPacket);

	}
}

void CIOCPserver::onPBroadCastEnterRoom(CPacket & packet)
{
	{
		std::string str = ViewUserInformation(CUserManager::getInst()->findUser(packet.getSocketNumber())->second);
		for (auto &player : CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getPool())
		{
			if (player.second->m_socket == packet.getSocketNumber())
				continue;

			CPacket sendPacket(P_BROADCAST_ENTER_ROOM_ACK);
			sendPacket.SetSocketNumber(player.second->m_socket);
			sendPacket << str;
			sendMessageQue.messageQue.push(sendPacket);
		}
	}
}

void CIOCPserver::onPReadyReq(CPacket & packet)
{
	for (auto &player : CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getPool())
	{
		CPacket sendPacket(P_READY_ACK);
		sendPacket.SetSocketNumber(player.second->m_socket);
		sendPacket << L"레디하시려면 1을 입력해주세요. \n";
		sendMessageQue.messageQue.push(sendPacket);
	}
}

void CIOCPserver::onPReadyResult(CPacket & packet)
{
	int iInput;

	packet >> iInput;

	if (iInput == 1)
		CUserManager::getInst()->findUser(packet.getSocketNumber())->second->eStatus = Ready;

	std::string str = ViewUserInformation(CUserManager::getInst()->findUser(packet.getSocketNumber())->second);



	CPacket sendPacket(P_READYRESULT_ACK);
	sendPacket.SetSocketNumber(packet.getSocketNumber());
	sendPacket << str;
	sendMessageQue.messageQue.push(sendPacket);
}

void CIOCPserver::onPGameStartReady(CPacket & packet)
{
	/*for (auto &player : CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getPool())
	{
		if (player.second->eStatus == Ready)
		{
			CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->setReadyCnt();
			player.second->eStatus = GameStart;
		}
	}
	if (CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->GetReadyCnt() == 2)
	{
		for (auto &player : CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getPool())
		{

			CPacket sendPacket(P_GAMESTARTREADY_ACK);
			sendPacket.SetSocketNumber(player.second->m_socket);
			sendPacket << L"Game Start";
			sendMessageQue.messageQue.push(sendPacket);

		}
		CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->GameStart();
	}*/
}

void CIOCPserver::onPGameStart(CPacket & packet)
{
	if (CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iTeam == 1)
	{
		CPacket sendPacket(P_GAMESTART_ACK);
		sendPacket.SetSocketNumber(packet.getSocketNumber());
		sendPacket << 1;
		sendMessageQue.messageQue.push(sendPacket);
	}
	else if (CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iTeam == 2)
	{
		CPacket sendPacket(P_GAMESTART_ACK);
		sendPacket.SetSocketNumber(packet.getSocketNumber());
		sendPacket << 2;
		sendMessageQue.messageQue.push(sendPacket);
	}
}

void CIOCPserver::onPGameInputKey(CPacket & packet)
{
	wchar_t cInput[5];
	CPosition pos;
	packet >> cInput >> pos;

	char* pStr;
	int strSize = WideCharToMultiByte(CP_ACP, 0, cInput, -1, NULL, 0, NULL, NULL);
	pStr = new char[strSize];
	WideCharToMultiByte(CP_ACP, 0, cInput, -1, pStr, strSize, 0, 0);

	std::string asdf;
	for (int i = 0; i < sizeof(pStr); i++)
	{
		asdf += pStr[i];
	}
	bool bAbleCheck = playerPositionSetting(asdf, pos, packet);
	for (auto &player : CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getPool())
	{
		if (player.first == packet.getSocketNumber())
		{
			CPacket sendPacket(P_GAMEINPUT_ACK);
			sendPacket.SetSocketNumber(packet.getSocketNumber());
			sendPacket << bAbleCheck << cInput;
			sendMessageQue.messageQue.push(sendPacket);
		}

		else {
			CPacket sendPacket(P_ENEMYPOS_ACK);
			sendPacket.SetSocketNumber(player.first);
			sendPacket << bAbleCheck << cInput;
			sendMessageQue.messageQue.push(sendPacket);

		}
	}
}

bool CIOCPserver::playerPositionSetting(std::string cInputKey, CPosition pos, CPacket & packet)
{
	char inputKey = cInputKey[0];
	bool bAbleMove = true;
	if (CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iTeam == 1)
	{
		CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getStage()->m_Player[0].SetPlayerpos(pos.x, pos.y);
		CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getStage()->m_Player[0].SetRoomNumber(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum);
		bAbleMove = CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getStage()->CheckMove(CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getStage()->m_Stage, CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getStage()->m_Player[0], inputKey);
	}
	else if (CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iTeam == 2)
	{
		CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getStage()->m_Player[1].SetPlayerpos(pos.x, pos.y);
		CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getStage()->m_Player[1].SetRoomNumber(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum);
		bAbleMove = CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getStage()->CheckMove(CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getStage()->m_Stage, CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second->iRoomNum)->getStage()->m_Player[1], inputKey);


	}

	return bAbleMove;
}

void CIOCPserver::MatchingCheck()
{
	if (matchingQue.size() % 2 == 0 )
	{
		
		CGameRoom GameRoom;
		CRoomManager::getinst()->insertRoom(GameRoom);
		CRoomManager::getinst()->findRoom(GameRoom.GetRoomNumber())->insertUserPool(CUserManager::getInst()->findUser(matchingQue.front()->m_socket)->second);
		CUserManager::getInst()->findUser(matchingQue.front()->m_socket)->second->eStatus = GameStart;
		CUserManager::getInst()->findUser(matchingQue.front()->m_socket)->second->iTeam = 1;	
		matchingQue.pop();

		CRoomManager::getinst()->findRoom(GameRoom.GetRoomNumber())->insertUserPool(CUserManager::getInst()->findUser(matchingQue.front()->m_socket)->second);
		CUserManager::getInst()->findUser(matchingQue.front()->m_socket)->second->eStatus = GameStart;
		CUserManager::getInst()->findUser(matchingQue.front()->m_socket)->second->iTeam = 2;
		matchingQue.pop();

		for(auto user : CRoomManager::getinst()->findRoom(GameRoom.GetRoomNumber())->getPool())
		{ 
			if (user.second->iTeam == 1)
			{
				CPacket sendPacket(P_GAMESTART_ACK);
				sendPacket.SetSocketNumber(user.second->m_socket);
				sendPacket << 1;
				sendMessageQue.messageQue.push(sendPacket);
			}
			else
			{
				CPacket sendPacket(P_GAMESTART_ACK);
				sendPacket.SetSocketNumber(user.second->m_socket);
				sendPacket << 2;
				sendMessageQue.messageQue.push(sendPacket);
			}
		
		}
		CRoomManager::getinst()->findRoom(GameRoom.GetRoomNumber())->GameStart();
	}
}

void CIOCPserver::ChoiceLobbyOption(wchar_t iNum[2], SOCKET socket)
{
	char* pStr;
	int strSize = WideCharToMultiByte(CP_ACP, 0, iNum, -1, NULL, 0, NULL, NULL);
	pStr = new char[strSize];
	WideCharToMultiByte(CP_ACP, 0, iNum, -1, pStr, strSize, 0, 0);

	std::string asdf;
	for (int i = 0; i < sizeof(pStr); i++)
	{
		asdf += pStr[i];
	}

	int num = atoi(asdf.c_str());
	switch (num)
	{
	case 1:
	{
		/*CGameRoom GameRoom;
		CRoomManager::getinst()->insertRoom(GameRoom);
		CRoomManager::getinst()->findRoom(GameRoom.GetRoomNumber())->insertUserPool(CUserManager::getInst()->findUser(socket)->second);*/
		matchingQue.push(CUserManager::getInst()->findUser(socket)->second);
		CUserManager::getInst()->findUser(socket)->second->eStatus = InRoom;
		CUserManager::getInst()->findUser(socket)->second->iTeam = 1;

	}
	break;
	}
}



std::string CIOCPserver::ViewUserInformation(Socket* User)
{
	std::string temp;

	for (auto player : CRoomManager::getinst()->findRoom(User->iRoomNum)->getPool())
	{
		temp += "유저 아이디 : ";
		temp += (char*)player.second->ID;
		temp += "\t";
		temp += "유저 레벨 : ";
		temp += std::to_string(player.second->iLevel);
		temp += "\t";
		temp += "유저 상태 : ";
		temp += (VIewUserStatuInformation(player.second));
		temp += "\t";
		temp += "\n";
	}
	return temp;
}

std::string CIOCPserver::VIewUserStatuInformation(Socket* User)
{
	switch (User->eStatus)
	{
	case 1:
		return "Lobby";
	case 2:
		return "InRoom";
	case 3:
		return "Ready";
	case 4:
		return "GameStart";
	}
}

