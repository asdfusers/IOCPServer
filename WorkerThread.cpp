#include "stdafx.h"
#include "WorkerThread.h"
#include "Connection.h"
WorkerThread::WorkerThread()
{
}


WorkerThread::~WorkerThread()
{
}

void WorkerThread::threadMain()
{
	DWORD byteTransfer, completionKey;
	LPOVERLAPPED overlapped;
	CConnection* Connection;
	while (true)
	{	
		if (GetQueuedCompletionStatus(m_hcp, &byteTransfer, &completionKey, (LPOVERLAPPED*)&Connection, INFINITE) ==0)
		{
			std::cout << "Failed()" << std::endl;
			closesocket(Connection->getSocket());
			continue;
		}
		Connection->wsaBuf.len= byteTransfer;
		XTrace(L"%s", Connection->wsaBuf.buf);
		
		if (byteTransfer == 0)
		{
			closesocket(Connection->getSocket());
			continue;
		}

	}
}
