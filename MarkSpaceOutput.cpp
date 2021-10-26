

#include "MarkSpaceOutput.h"
#include "inifile.h"
#include "ProjectLogger.h"



CMarkSpaceOutput::CMarkSpaceOutput(int pin, bool bDefault) 
: COutPin(pin, bDefault)
{
	m_pAppSettings = new INIReader(INIPATH);
	Start()	;
}


CMarkSpaceOutput::~CMarkSpaceOutput()
{
	if(ThreadIsRunning())
	{
		Stop();
	}
	SetState(false);
	delete m_pAppSettings;
}

int CMarkSpaceOutput::Run()
{
	unsigned int dwOnTime = 0;
	unsigned int dwOffTime = 0;
	while(!ShouldExit(10))
	{
		if(GetState())
		{
			dwOnTime++;
			unsigned int dwTimeToBeOn;
			dwTimeToBeOn = m_pAppSettings->GetInteger(MARKSPACE_SEC, MARKSPACE_ON,1);
			dwTimeToBeOn *= 100;
			if(dwOnTime >= dwTimeToBeOn)
			{
				//CProjectLogger::GetInstance()->Log("MarkSpace OFF\n");
				SetState(false);
				dwOffTime = 0;
			}
		}
		else
		{
			dwOffTime++;
			unsigned int dwTimeToBeOff;
			dwTimeToBeOff = m_pAppSettings->GetInteger(MARKSPACE_SEC, MARKSPACE_OFF,1);
			dwTimeToBeOff *= 100;
			if(dwOffTime >= dwTimeToBeOff)
			{
				//CProjectLogger::GetInstance()->Log("MarkSpace ON\n");
				SetState(true);
				dwOnTime = 0;
			}
		}
	}
	return 0;
}

