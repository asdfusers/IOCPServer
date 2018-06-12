// IOCP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IOCPserver.h"

int main()
{
	CIOCPserver server;
	server.Init(9000);
	while (true)
	{
		server.Update();
	}
	server.UnInit();
    return 0;
}

