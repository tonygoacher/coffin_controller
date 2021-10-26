//
// A class to play the specified sample and trigger an output for n seconds when an input is detected
#ifndef __DOORBELL_H
#define __DOORBELL_H


#include "INIReader.h"
#include "SDLBlockedSample.h"
#include "TimedOutput.h"
#include "PinMonitor.h"
#include "Thread.h"
#include "MonitoredPin.h"
#include <semaphore.h>
#include "INIReader.h"

class CDoorBell: public CMonitoredPinClient, public CThread
{
public:
	virtual ~CDoorBell();
	static CDoorBell* GetInstance();
			
	CMonitoredPin*	GetDoorbellPin() { return m_pPin;}
	
	int Run();
protected:
	CDoorBell();
	virtual void PinChanged(CMonitoredPin* pPin);
	INIReader*	m_ApplicationSettings;
	CSDLBlockedSample		m_BlockedSample;
	CTimedOutput*			m_pTimedOutput;
	sem_t					m_sem;
	CMonitoredPin*			m_pPin;
	static CDoorBell*		m_pThis;
	COutPin*				m_pHydra;

private:
	 void StopEvent();	// Called after Stop is signalled, do anything else required to stop your thread here
	 void PlayBlocked();
	 bool m_SamplePlaying;
};

#endif
