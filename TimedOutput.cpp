

#include "TimedOutput.h"
#include "inifile.h"
#include "ProjectLogger.h"
#include <unistd.h>


CTimedOutput::CTimedOutput(int pin, bool bDefaultState)
: COutPin(pin, bDefaultState)
{

	sem_init(&m_sem,0,0);
	Start();
	m_bDefault = bDefaultState;
	m_pAppSettings = new INIReader(INIPATH);
	m_dwTimePeriod = m_pAppSettings->GetInteger(TIMINGS_SEC,TIMINGS_STROBE,2500);
}

CTimedOutput::~CTimedOutput()
{
	
	Stop(&m_sem);
	delete m_pAppSettings;
}

void CTimedOutput::StopEvent()
{

	sem_post(&m_sem);
}

void CTimedOutput::Fire()
{
	sem_post(&m_sem);
}


int CTimedOutput::Run()
{
	while(!ShouldExit())
	{
		sem_wait(&m_sem);
		if(!ShouldExit())
		{
			//CProjectLogger::GetInstance()->Log("Timed output ON\n");
			SetState(true);
			sleep(m_dwTimePeriod);
			//CProjectLogger::GetInstance()->Log("Timed output OFF\n");
			SetState(false);
		}
	}
	return 0;
}
