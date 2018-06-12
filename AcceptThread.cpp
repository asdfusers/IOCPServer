#include "stdafx.h"
#include "AcceptThread.h"


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
	struct timeval tv = { 0, 10 };
	FD_SET rds;
	int retval;
	while (true)
	{
		FD_ZERO(&rds);
		FD_SET(m_ListenSocket, &rds);
		retval = select(0, &rds, NULL, NULL, &tv);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
				std::cout << "accept()" << std::endl;
			continue;
		}
		else if (FD_ISSET(m_ListenSocket, &rds))
		{
			Connection = accept(m_ListenSocket, (SOCKADDR*)&sockAddr, &addrLen);
			std::cout << "[서버] 클라이언트 접속 : IP[ " << inet_ntoa(sockAddr.sin_addr) << " ], \t 포트번호[ " << ntohs(sockAddr.sin_port) << " ]" << std::endl;
			
			
			CConnection* socketInfo = new CConnection;
			memset(socketInfo, 0x00, sizeof(CConnection));
			socketInfo->recvBytes = 0;
			socketInfo->sendBytes = 0;
			socketInfo->wsaBuf.len = 1024;
			socketInfo->wsaBuf.buf = socketInfo->buffer;
			socketInfo->m_socket = Connection;
			DWORD receivedBytes =0,flags =0;
			
			m_SocketList.push_back(socketInfo);
			m_hcp = CreateIoCompletionPort((HANDLE)Connection, m_hcp, (DWORD)socketInfo, 0);

			if (WSARecv(socketInfo->getSocket(), &socketInfo->wsaBuf, 1, &receivedBytes,
				&flags, &(socketInfo->getOverlapped()), NULL))
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					std::cout << "Error - IO pending Failure" << std::endl;
					return;
				}
			}
			
		}

	}
}
