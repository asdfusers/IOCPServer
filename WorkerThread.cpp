#include "stdafx.h"
#include "WorkerThread.h"
#include "Socket.h"
WorkerThread::WorkerThread()
{
}


WorkerThread::~WorkerThread()
{
}

void WorkerThread::threadMain()
{
	LPOVERLAPPED overlapped;
	DWORD byteTransfer, completionKey=0;
	Socket* Socket;
	int retval;
	DWORD flags = 0;

	while (true)
	{

		retval = GetQueuedCompletionStatus(m_hcp, &byteTransfer, (LPDWORD)&Socket, (LPOVERLAPPED*)&overlapped, INFINITE);
		DWORD dwError = GetLastError();

		if (retval == 0 || byteTransfer == 0)
		{
			if (retval == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(Socket->getSocket(), &Socket->overlapped, &temp1, FALSE, &temp2);
				std::cout << "WSAGetoverlappedResulT()" << std::endl;
			}
			closesocket(Socket->getSocket());
			delete Socket;
			Socket = NULL;
			continue;
		}

		onReceive(byteTransfer, Socket);
		ZeroMemory(&Socket->overlapped, sizeof(Socket->overlapped));
		Socket->recvBytes = Socket->sendBytes = 0;
		Socket->wsaBuf.buf = Socket->buffer;
		WSARecv(Socket->getSocket(), &Socket->wsaBuf, 1, &byteTransfer, &flags, &(Socket->overlapped), NULL);
	

}

}

void WorkerThread::onReceive(DWORD bytesTransferred, Socket* socket)
{
	CPacket receivedPacket;

	socket->recvBytes = bytesTransferred;
	while (socket->recvBytes > 0)
	{
		receivedPacket.copyToBuffer(socket->wsaBuf.buf, socket->recvBytes);

		if (receivedPacket.isValidPacket() == true && socket->recvBytes >= (int)receivedPacket.getPacketSize())
		{
			char buffer[PACKETBUFFERSIZE];
			CCriticalSectionLock cs(cs);
			receivedPacket.SetSocketNumber(socket->getSocket());
			socket->getRecvQue().messageQue.push(receivedPacket);
			socket->recvBytes -= receivedPacket.getPacketSize();
		}
		else
			break;
	}

}
