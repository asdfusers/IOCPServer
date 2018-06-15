#include "stdafx.h"
#include "AcceptThread.h"
#include "Packet.h"
#include "UserManager.h"

CAcceptThread::CAcceptThread()
{
}


CAcceptThread::~CAcceptThread()
{
}

void CAcceptThread::threadMain()
{
	SOCKET Connection;
	SOCKADDR_IN sockAddr;
	int addrLen = sizeof(sockAddr);
	int retval = 0;
	DWORD receivedBytes;
	DWORD flags;

	while (true)
	{
	
		
		Connection = WSAAccept(m_ListenSocket, (SOCKADDR*)&sockAddr, &addrLen, NULL, 0);
		
		if (Connection == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
				std::cout << "accept()" << std::endl;
			continue;
		}
		
		std::cout << "[서버] 클라이언트 접속 : IP[ " << inet_ntoa(sockAddr.sin_addr) << " ], \t 포트번호[ " << ntohs(sockAddr.sin_port) << " ]" << std::endl;


		m_hcp = CreateIoCompletionPort((HANDLE)Connection, m_hcp, Connection, 0);

		CConnection socketInfo;
		ZeroMemory(&socketInfo.overlapped, sizeof(socketInfo.overlapped));
		socketInfo.recvBytes = socketInfo.sendBytes = 0;
		socketInfo.wsaBuf.buf = socketInfo.buffer;
		socketInfo.wsaBuf.len = PACKETBUFFERSIZE;
		socketInfo.m_socket = Connection;

		XTrace(L"%d", socketInfo);

		flags = 0;
		
		retval = WSARecv(socketInfo.m_socket, &socketInfo.wsaBuf, 1, &receivedBytes,
			&flags, &socketInfo.overlapped, NULL);
	


		CCriticalSectionLock cs(cs);
		m_SocketList.push_back(socketInfo);
		CUserManager::getInst()->insertUser(socketInfo);

		CPacket sendPacket(P_CONNECTIONSUCCESS_ACK);
		sendPacket << L"Welcome To Network GameLobby \nPlease Input Your ID and Password\n";
		sendMessage(sendPacket, socketInfo.getSocket());

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("%d \n ", WSAGetLastError());
				return;
			}
			continue;
		}
		
		

		


	
	}
}

bool CAcceptThread::sendMessage(CPacket & packet, SOCKET SOCK)
{
	int retVal = send(SOCK, packet.getPacketBuffer(), packet.getPacketSize(), 0);
	if (retVal == SOCKET_ERROR)
		return false;
	return true;
}

