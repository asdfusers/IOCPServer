#include "stdafx.h"
#include "IOCPserver.h"

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
	return false;
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

	/*int reuse = 1;
	setsockopt(m_ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));

	unsigned long arg = 1;
	ioctlsocket(m_ListenSocket, FIONBIO, &arg);*/

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
	/*DWORD receivedBytes, flags = 0;
	CCriticalSectionLock cs(acceptThread.cs);
	if (!acceptThread.getSocketList().empty())
	{
		if (WSARecv(acceptThread.getSocketList().begin()->m_socket, &acceptThread.getSocketList().begin()->wsaBuf, 1, &receivedBytes,
			&flags, &(acceptThread.getSocketList().begin()->overlapped), NULL))
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				std::cout << "Error - IO pending Failure" << std::endl;
				return;
			}
		}
	}*/
}

void CIOCPserver::AcceptThreadInit()
{
	acceptThread.SetListenSocket(m_ListenSocket);
	acceptThread.Set_IOCP_Handle(hcp);
	acceptThread.begin();
}

