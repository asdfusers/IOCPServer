#pragma once
#include "stdafx.h"
#include "Packet.h"
#include "Socket.h"


class CConnection
{
public:
	CConnection();
	~CConnection();

	void CloseSocket();
	void onDestroy();

public:
	Socket* m_Socket;

};

