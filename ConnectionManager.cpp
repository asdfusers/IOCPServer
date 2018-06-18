#include "stdafx.h"
#include "ConnectionManager.h"

CConnectionManager* CConnectionManager::selfIns = NULL;
void CConnectionManager::insertConnection(CConnection * Connection)
{
	CCriticalSectionLock cs(cs);
	connectionList.push_back(Connection);
}
void CConnectionManager::closeConnection(Socket* socket)
{
	CCriticalSectionLock cs(cs);
	itr = connectionList.begin();
	CConnection* item = *itr;
	while (itr != connectionList.end())
	{
		if (item->m_Socket == socket)
			break;
		itr++;
	}
	item->onDestroy();
//	delete item;

	connectionList.erase(itr);


}
void CConnectionManager::removeConnection(CConnection * Connection)
{
	CCriticalSectionLock cs(cs);
	itr = connectionList.begin();
	while (itr != connectionList.end())
	{
		CConnection* item = *itr;
		if (item == Connection)
			break;
		itr++;
	}
	Connection->onDestroy();
	delete Connection;

	connectionList.erase(itr);

	
}
CConnectionManager::CConnectionManager()
{
}


CConnectionManager::~CConnectionManager()
{
	CCriticalSectionLock cs(cs);
	std::list<CConnection*>::iterator itr2;

	itr = connectionList.begin();
	while (itr != connectionList.end())
	{
		itr2 = itr;
		CConnection* connection = *itr;
		itr++;
		connection->onDestroy();
			delete connection;
		connectionList.erase(itr2);
	}

}
