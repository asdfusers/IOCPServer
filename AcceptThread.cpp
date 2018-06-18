#include "stdafx.h"
#include "AcceptThread.h"
#include "Packet.h"
#include "UserManager.h"
#include "ConnectionManager.h"
#include "Socket.h"
CAcceptThread::CAcceptThread()
{
}


CAcceptThread::~CAcceptThread()
{
}

void CAcceptThread::threadMain()
{
	SOCKET connect;
	SOCKADDR_IN sockAddr;
	int addrLen = sizeof(sockAddr);
	int retval = 0;
	DWORD receivedBytes;
	DWORD flags;

	while (true)
	{	
		connect = WSAAccept(m_ListenSocket, (SOCKADDR*)&sockAddr, &addrLen, NULL, 0);
		
		if (connect == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
				std::cout << "accept()" << std::endl;
			continue;
		}
		
		std::cout << "[서버] 클라이언트 접속 : IP[ " << inet_ntoa(sockAddr.sin_addr) << " ], \t 포트번호[ " << ntohs(sockAddr.sin_port) << " ]" << std::endl;

		CConnection* connection = new CConnection;
		CConnectionManager::getInst()->insertConnection(connection);
		Socket* SockettInfo = new Socket;
		if (SockettInfo == NULL)
		{
			break;
		}
		
		ZeroMemory(&SockettInfo->overlapped, sizeof(SockettInfo->overlapped));
		SockettInfo->recvBytes = SockettInfo->sendBytes = 0;
		SockettInfo->wsaBuf.buf = SockettInfo->buffer;
		SockettInfo->wsaBuf.len = PACKETBUFFERSIZE;
		SockettInfo->m_socket = connect;
		SockettInfo->ioType = IO_READ;
	
		connection->m_Socket = SockettInfo;
		
		m_hcp = CreateIoCompletionPort((HANDLE)connect, m_hcp,(DWORD)SockettInfo, 0);


		flags = 0;
		
		retval = WSARecv(SockettInfo->m_socket, &SockettInfo->wsaBuf, 1, &receivedBytes,
			&flags, (LPWSAOVERLAPPED)&SockettInfo->overlapped, NULL);
		
		if (retval == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
			CloseClient(SockettInfo);
		
		
		{
			CCriticalSectionLock cs(cs);
			CUserManager::getInst()->insertUser(SockettInfo);	
		}
		

		CPacket sendPacket(P_CONNECTIONSUCCESS_ACK);
		sendPacket << L"Welcome To Network GameLobby \nPlease Input Your ID and Password\n";
		sendMessage(sendPacket, SockettInfo->m_socket);


	}
}

void CAcceptThread::CloseClient(Socket * socket)
{
	if (!socket)
		return;
	closesocket(socket->m_socket);
	delete socket;
}

bool CAcceptThread::sendMessage(CPacket & packet, SOCKET SOCK)
{
	int retVal = send(SOCK, packet.getPacketBuffer(), packet.getPacketSize(), 0);
	if (retVal == SOCKET_ERROR)
		return false;
	return true;
}

