#pragma once
#include "Thread.h"
#include "Connection.h"

class CAcceptThread : public CThread
{
public:
	CAcceptThread();
	~CAcceptThread();

	virtual void threadMain();
	void CloseClient(Socket *socket);
	bool sendMessage(CPacket& packet, SOCKET SOCK);

	void SetListenSocket(SOCKET _socket) { m_ListenSocket = _socket; }
	void Set_IOCP_Handle(HANDLE hcp) { m_hcp = hcp; }
	CConnection& getConnection() { return Connection; }
	
	CS::CriticalSection cs;

private:
	SOCKET m_ListenSocket;
	HANDLE m_hcp;
	CConnection Connection;

};

