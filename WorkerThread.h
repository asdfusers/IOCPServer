#pragma once
#include "Thread.h"
#include "CriticalSections.h"
class WorkerThread : public CThread
{
public:
	WorkerThread();
	~WorkerThread();

	virtual void threadMain();

	void Set_IOCP_HANDLE(HANDLE hcp) { m_hcp = hcp; }

private:
	HANDLE m_hcp;

};

