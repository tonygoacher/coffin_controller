#ifndef  __TIMEDOUTPUT_H
#define __TIMEDOUTPUT_H

#include "SDLBlockedSample.h"
#include "Thread.h"

#include "OutPin.h"

#include "INIReader.h"
#include <semaphore.h>


class CTimedOutput : public CThread, protected COutPin
{
public:
	CTimedOutput(int pin, bool bDefault);
	virtual ~CTimedOutput();
	void Fire();

protected:
	virtual int Run();
	void StopEvent();
	sem_t		m_sem;
	unsigned int	m_dwTimePeriod;
	bool		m_bDefault;
	INIReader*	m_pAppSettings;

};



#endif
