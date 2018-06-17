#pragma once
#include "stdafx.h"
#include "Connection.h"
class CRoom
{
public:
	CRoom();
	~CRoom();


public:
	virtual void insertUserPool(Socket* User);
	virtual void deleteUserPool(Socket* User) = 0;
	virtual Socket* findUserPool(SOCKET socket) = 0;

public:

};

