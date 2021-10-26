
#include "Thread.h"
#include <unistd.h>

#ifndef TRACE
#define TRACE OutputDebugString
#endif

CThread::CThread(const char *szThreadName)
{


	m_hThread = -1;
	m_bThreadRunning = false;


	m_bThreadShouldStop = false;

	if(szThreadName != NULL)
	{
		m_ThreadName = szThreadName;
	}


}

CThread::~CThread(void)
{
	

}

bool CThread::Start()
{
	m_pThis = this;
	m_bThreadRunning = true;
	return pthread_create(&m_hThread, NULL, &CThread::staticThread, this) == 0;

}

bool CThread::ShouldExit(int delay)
{
	usleep(delay * 1000);
	return m_bThreadShouldStop;
}

void CThread::Stop(sem_t* sem)
{
	m_bThreadShouldStop = true;
	if(sem)
	{
		sem_post(sem);
	}
	while(m_bThreadShouldStop)
	{
		usleep(100);
	}
	
}

void CThread::DeleteHandles()
{


}

int CThread::Thread()
{

	int returnValue = Run();
	
	m_bThreadRunning = false;

	return returnValue;
}

void* CThread::staticThread(void* lpParameters)
{
	const CThread *pParameters = (const CThread*) lpParameters;

	(void*)pParameters->m_pThis->Thread();
	pParameters->m_pThis->m_bThreadRunning = false;
	pParameters->m_pThis->m_bThreadShouldStop = false;
	return lpParameters;
}
