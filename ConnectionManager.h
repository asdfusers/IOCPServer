#pragma once
#include "stdafx.h"
#include "Connection.h"

class CConnectionManager
{
public:

	static CConnectionManager* getInst()
	{
		if (selfIns == NULL)
			selfIns = new CConnectionManager;
		return selfIns;
	}


	void insertConnection(CConnection* Connection);
	void closeConnection(Socket* socket);
	void removeConnection(CConnection* Connection);
	std::list<CConnection*> GetConnectionList() { return connectionList; }


	CConnectionManager();
	~CConnectionManager();

	CS::CriticalSection cs;
private:
	std::list<CConnection*> connectionList;
	std::list<CConnection*>::iterator itr;
	static CConnectionManager* selfIns;
};

