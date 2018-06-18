#pragma once
#include "Thread.h"
#include "CriticalSections.h"
#include "Connection.h"
#include "CMessageQue.h"

class WorkerThread : public CThread
{
public:
	WorkerThread();
	~WorkerThread();

	virtual void threadMain();

	void Set_IOCP_HANDLE(HANDLE hcp) { m_hcp = hcp; }
	std::list<CConnection>& getConnectionList() { return ConnectionList; }
	CMessageQue& getRecvQue() { return recvQue; }
	
	void SetConnectionList(std::list<CConnection> _Connection) { ConnectionList = _Connection;}


private:
	HANDLE m_hcp;
	CMessageQue recvQue;
	std::list<CConnection> ConnectionList;
	CS::CriticalSection cs;
};

