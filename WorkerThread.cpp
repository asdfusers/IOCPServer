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
	int retval;
	DWORD flags = 0;

	while (true)
	{
		Socket* Socket;

		retval = GetQueuedCompletionStatus(m_hcp, &byteTransfer, (LPDWORD)&Socket, (LPOVERLAPPED*)&overlapped, INFINITE);
		DWORD dwError = GetLastError();

		if ( byteTransfer == 0)
		{
			if (retval == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(Socket->m_socket, &Socket->overlapped, &temp1, FALSE, &temp2);
				std::cout << "WSAGetoverlappedResult()" << std::endl;
			}
			Socket->closeConnection();
			delete Socket;
			//Socket = NULL;
			continue;
		}
		switch (Socket->ioType)
		{
		case IO_WRITE:
		{
			//Socket->onReceive(byteTransfer);
		}
			break;
		case IO_READ:
		{
			Socket->onReceive(byteTransfer);
		}
			break;
		}
		
		ZeroMemory(&Socket->overlapped, sizeof(Socket->overlapped));
		Socket->recvBytes = Socket->sendBytes = 0;
		Socket->wsaBuf.buf = Socket->buffer;
		WSARecv(Socket->m_socket, &Socket->wsaBuf, 1, &byteTransfer, &flags, &(Socket->overlapped), NULL);
	

}

}

