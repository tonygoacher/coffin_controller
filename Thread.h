#pragma once

#include <pthread.h>
#include <string>
#include <semaphore.h>

class CThread
{
protected:
	CThread(const char *szThreadName=0);
	virtual ~CThread(void);

public:
	bool Start();
	void Stop(sem_t* sem = NULL);

protected:
	virtual int Run() = 0;	// Override this function in derived classes


	bool ThreadIsRunning() const { return m_hThread!=0; }
	bool ShouldExit(int delay=0);
	
private:
	
	pthread_t	m_hThread;
	
	std::string m_ThreadName;
	
	bool m_bThreadRunning;
	bool m_bThreadShouldStop;

	CThread *m_pThis;


	
	int Thread();
	static void* staticThread(void* lpParameters);

	void DeleteHandles();	// Delete event and thread handles
};
