#include "stdafx.h"
#include "Connection.h"


CConnection::CConnection() 
{
	
}


CConnection::~CConnection()
{
	
}

void CConnection::CloseSocket()
{
}

void CConnection::onDestroy()
{
	this->CloseSocket();
	delete this;
}
