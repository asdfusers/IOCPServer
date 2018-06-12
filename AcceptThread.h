#pragma once
#include "Thread.h"
#include "Connection.h"

class CAcceptThread : public CThread
{
public:
	CAcceptThread();
	~CAcceptThread();

	virtual void threadMain();
	
	void SetListenSocket(SOCKET _socket) { m_ListenSocket = _socket; }
	void Set_IOCP_Handle(HANDLE hcp) { m_hcp = hcp; }
	std::list<CConnection*> getSocketList() { return m_SocketList; }
	
	CS::CriticalSection cs;
private:
	SOCKET m_ListenSocket;
	HANDLE m_hcp;
	std::list<CConnection*> m_SocketList;

};

