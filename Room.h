#pragma once
#include "stdafx.h"
#include "Connection.h"
class CRoom
{
public:
	CRoom();
	~CRoom();


public:
	virtual void insertUserPool(CConnection User);
	virtual void deleteUserPool(CConnection User) = 0;
	virtual CConnection findUserPool(SOCKET socket) = 0;

public:

};

