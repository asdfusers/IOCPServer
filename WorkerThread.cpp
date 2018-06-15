#include "stdafx.h"
#include "WorkerThread.h"

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
	SOCKET sock;
	CConnection* connection;
	bool retval;
	


	while (true)
	{

		retval = GetQueuedCompletionStatus(m_hcp, &byteTransfer, (LPDWORD)&sock, (LPOVERLAPPED*)&connection, INFINITE);
		DWORD dwError = GetLastError();

		if (retval)
		{

			XTrace(L"%d", connection->m_socket);
			connection->recvBytes = byteTransfer;
			onReceive(byteTransfer, connection);

			ZeroMemory(&connection->overlapped, sizeof(OVERLAPPED));
			DWORD flags = 0;

			WSARecv(connection->getSocket(), &connection->wsaBuf, 1, &byteTransfer, &flags, &(connection->getOverlapped()), NULL);
		}
		//else
		//{
		//	if (connection->getOverlapped() != NULL)
		//	{
		//		//실패완료 처리
		//	}
		//	else
		//	{
		//		if (dwError == WAIT_TIMEOUT)
		//		{
		//		}
		//		else
		//	}
		//}



		}

}

void WorkerThread::onReceive(DWORD bytesTransferred, CConnection* connection)
{
	CPacket receivedPacket;

	connection->SetRecvBytes(bytesTransferred);
	while (connection->getRecvBytes() > 0)
	{
		receivedPacket.copyToBuffer(connection->wsaBuf.buf, connection->getRecvBytes());

		if (receivedPacket.isValidPacket() == true && connection->getRecvBytes() >= (int)receivedPacket.getPacketSize())
		{
			char buffer[PACKETBUFFERSIZE];
			CCriticalSectionLock cs(cs);
			receivedPacket.SetSocketNumber(connection->getSocket());
			recvQue.messageQue.push(receivedPacket);
			connection->SetRecvBytes(connection->getRecvBytes() - receivedPacket.getPacketSize());
		}
		else
			break;
	}

}
