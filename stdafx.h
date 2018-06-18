// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <WinSock2.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <process.h>
#include <conio.h>

#include "CriticalSectionLock.h"
#include "CriticalSections.h"


using namespace std;

#define WorkerThreadCount 8

typedef enum {
	IO_READ,
	IO_WRITE
}IO_OPERATION;

// TODO: reference additional headers your program requires here
