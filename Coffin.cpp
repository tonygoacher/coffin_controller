
#include <mutex>
#include "Coffin.h"
#include "PinMonitor.h"
#include "Doorbell.h"
#include "MonitoredPin.h"
#include "SDLAudio.h"
#include "SoundList.h"
#include "ProjectLogger.h"
#include <unistd.h>
#include <algorithm>
#include "pig.h"


CCoffin* CCoffin::m_pThis = NULL;
std::mutex	 CCoffin::m_InstanceMutex;



CCoffin::CCoffin()
{
	m_eState = eResetSkeletonState;

	PIGPIO::Instance();
	
	m_ApplicationSettings = new INIReader("halloween.ini"); 


	int pin = m_ApplicationSettings->GetInteger("PINS","RESET", 8);
	m_pResetPin = new CMonitoredPin( pin, true);

	pin = m_ApplicationSettings->GetInteger("PINS","PIR", 24);
	m_pPIRPin = new CMonitoredPin(pin, true);


	m_pSkeletonDrivePin = new COutPin( 14, false);
	m_pCoffinLidDrivePin = new COutPin( 23, false);

	
	// Add this class as a client of out input pins
	CDoorBell::GetInstance()->GetDoorbellPin()->AddClient(this);
	m_pResetPin->AddClient(this);
	m_pPIRPin->AddClient(this);


	// And add our pins to the monitor
	CPinMonitor::GetInstance()->AddPin(m_pResetPin);
	CPinMonitor::GetInstance()->AddPin(m_pPIRPin);


	m_Event = CCoffin::None;

	m_bDemoMode = false;
	m_eMode = eRedMode;

	Start();
}

CCoffin* CCoffin::GetInstance()
{
	std::lock_guard<std::mutex> lock(m_InstanceMutex);
	if(m_pThis == NULL)
	{
		m_pThis = new CCoffin();
	}
	return m_pThis;
}

CCoffin::~CCoffin()
{
	m_Event = CCoffin::Shutdown;
	Stop();
	while(ThreadIsRunning())
	{
		sleep(100);
	}

	m_pResetPin->RemoveClient(this);
	CPinMonitor::GetInstance()->RemovePin(m_pResetPin);
	delete	m_pResetPin;
	
	m_pPIRPin->RemoveClient(this);
	CPinMonitor::GetInstance()->RemovePin(m_pPIRPin);
	delete	m_pPIRPin;

	
	ResetSkeletonState();
	ResetLidState();

	// Delete out pins
	delete	m_pSkeletonDrivePin;
	delete	m_pCoffinLidDrivePin;
 
	

}
// The pin changed handler
void CCoffin::PinChanged(CMonitoredPin* pPin)
{
	// If the doorbell pin has changed
	if(pPin == CDoorBell::GetInstance()->GetDoorbellPin() && pPin->GetState())
	{

		m_Event = CCoffin::Doorbell;
	}

	// If the reset pin has changed
	if(pPin == m_pResetPin && pPin->GetState())
	{
		CProjectLogger::GetInstance()->Log("RESET pin\n");
		m_Event = CCoffin::Reset;
	}

	// if the PIR pin has changed
	if(pPin == m_pPIRPin && pPin->GetState())
	{
		CProjectLogger::GetInstance()->Log("PIR\n");
		m_Event = CCoffin::PIR;
	}

}


void CCoffin::ChangeDemoMode(CInPin *pPin)
{

	// If the switch is set
	if(pPin->GetState())
	{
		// If already in demo mode, clear it
		std::lock_guard<std::mutex> ledLock(m_LedMutex);
	
		int iDelay;

		if(m_bDemoMode)
		{
			m_bDemoMode = false;
			iDelay = 400;

		}
		else
		{

			iDelay = 100;
			m_bDemoMode = true;
		}



		if(m_bDemoMode)
		{
			m_Event = CCoffin::Demo;
		}
		
	}
}

CCoffin::ECoffinState CCoffin::IdleState()
{
	ECoffinState eNextState = eIdleState;
	// Nothing to do but wait for the PIR event, doorbell event or shutdown

	

	m_bDemoMode = false;

	std::list<CCoffin::Event> listOfEvents;
	listOfEvents.push_back(CCoffin::PIR);
	listOfEvents.push_back(CCoffin::Doorbell);
	listOfEvents.push_back(CCoffin::Demo);
	listOfEvents.push_back(CCoffin::Shutdown);
	
	Event event = GetEvent(listOfEvents);

	
	if(event != None)
	{
	
		switch(event)
		{

		case	CCoffin::PIR:	// PIR
			// If in red or amber mode, start the coffin lid open sequence
			if((m_eMode == eAmberMode || m_eMode == eRedMode) && m_pPIRPin->GetState())
			{
				eNextState =  eCoffinLidOpeningPlaySampleState;		
			}
			break;


		case    CCoffin::Doorbell:	// Doorbell
			// If in red or amber mode, start the coffin lid open sequence
			if(m_eMode == eAmberMode || m_eMode == eRedMode)
			{
				eNextState =  eCoffinLidOpeningState;		
			}
			break;

		case	CCoffin::Shutdown:	eNextState = eShutdown;			// Shutdown event
			break;

		case	CCoffin::Demo:	eNextState = eCoffinLidOpeningPlaySampleState;	// Demo switch
			break;

		}
	}


	return eNextState;
}

CCoffin::ECoffinState CCoffin::ResetSkeletonState()
{
	// First reset the skeleton
	m_pSkeletonDrivePin->SetState(false);

	int iDelay = m_ApplicationSettings->GetInteger("COFFIN","SKELRESETDELAY", m_skiSkelResetDelay);
	
	printf("Resetting skeleton\n");
	sleep(iDelay);
	
	return eResetLidState;
}

CCoffin::ECoffinState CCoffin::ResetLidState()
{
	// Now reset the lid
	m_pCoffinLidDrivePin->SetState(false);
	int iDelay = m_ApplicationSettings->GetInteger("COFFIN","LIDRESETDELAY", m_skiLidResetDelay);

	sleep(iDelay);

	
	return ePIRResetState;
}


CCoffin::ECoffinState CCoffin::PIRResetState()
{
	ECoffinState eNextState = eIdleState;
	
	std::list<CCoffin::Event> listOfEvents;
	listOfEvents.push_back(CCoffin::PIR);
	listOfEvents.push_back(CCoffin::Shutdown);
	listOfEvents.push_back(CCoffin::Demo);	
	
	// Wait for the [ir to reset. It is an NC device
	while(m_pPIRPin->GetState())
	{
	

		
		CCoffin::Event event = GetEvent(listOfEvents);
		

		if(event == CCoffin::PIR)
		{
			CProjectLogger::GetInstance()->Log("Pir Reset complete\n");
		}
		if(event == CCoffin::Shutdown)
		{
			return eShutdown;
		}

		if(event == CCoffin::Demo)
		{
			eNextState = eCoffinLidOpeningPlaySampleState;	// Demo switch
		}
	}
	return eNextState;
}

CCoffin::ECoffinState CCoffin::CoffinLidOpeningPlaySampleState()
{
	// Play coffin sample
	CSoundList::GetInstance()->SetVolume(0);
	CSDLAudio::GetInstance()->PlaySample("creaky_door_4.wav", this);
	return CoffinLidOpeningState();
}

CCoffin::ECoffinState CCoffin::CoffinLidOpeningState()
{
	std::list<CCoffin::Event> listOfEvents;
	listOfEvents.push_back(CCoffin::Reset);
	listOfEvents.push_back(CCoffin::Shutdown);
	
		
	m_pCoffinLidDrivePin->SetState(true);


	// Wait for n seconds or reset signal or shutdown signal
	int iDelay = m_ApplicationSettings->GetInteger("COFFIN","LIDRISETIME", m_skiLidRiseTime);

	CCoffin::Event event = GetEvent(listOfEvents,iDelay);
	

	ECoffinState eNextState = eCoffinLidOpenState;

	if(event != CCoffin::None)
	{
		switch(event)
		{
		case	CCoffin::Reset:							// Reset event
					eNextState = eResetLidState;		
					break;

		case	CCoffin::Shutdown:	eNextState = eShutdown;	// Shutdown
					break;

		}
	}
	return eNextState;
}


void CCoffin::SampleFinished()
{
	CSoundList::GetInstance()->SetVolume(128);
}

CCoffin::ECoffinState CCoffin::CoffinLidOpenState()
{
	

	std::list<CCoffin::Event> listOfEvents;
	listOfEvents.push_back(CCoffin::Reset);
	listOfEvents.push_back(CCoffin::Shutdown);
	listOfEvents.push_back(CCoffin::Doorbell);	
	
	// Wait for n seconds or reset signal or shutdown signal
	unsigned int  dwTimeout = m_ApplicationSettings->GetInteger("COFFIN","LIDOPENTIMEOUT", m_skiLidOpenTimeout);

	Event event = GetEvent(listOfEvents,dwTimeout);


	ECoffinState eNextState = eCoffinLidOpenState;	// Remain in this state if an unknown event
	if(event != CCoffin::None)
	{
		switch(event)
		{

		case	CCoffin::Reset:	eNextState =  eResetLidState;	// Reset
					break;

		case	CCoffin::Shutdown:	eNextState = eShutdown;			// Shutdown
					break;

		case	CCoffin::Doorbell:	if(m_eMode == eRedMode || m_bDemoMode)
					{
						eNextState = eRaisingSkeletonState;;	// Doorbell
					}
					break;

		default:	eNextState = eCoffinLidOpenState;	// Remain in this state
		}
	}
	else
	{
		// Timed out, reset and close the lid
		eNextState =  eResetLidState;
	}
	return eNextState;

}


CCoffin::Event CCoffin::GetEvent(std::list<CCoffin::Event> events, int delay)
{
	m_Event = CCoffin::None;
	unsigned int delayCounter = delay * 100;
	while(delay ==0 || delayCounter != 0)
	{
		usleep(10000);
		if(delayCounter)
		{
			delayCounter--;
		}
		
		if(m_Event != CCoffin::None && std::find(events.begin() , events.end() , m_Event) != events.end())
		{
			printf("Got event %d\n", m_Event);
			return m_Event;
		}	
		else
		{
			m_Event = CCoffin::None;
		}
		
	}
	return m_Event;
}

CCoffin::ECoffinState CCoffin::SkeletonRaisingState()
{
	
	std::list<CCoffin::Event> listOfEvents;
	listOfEvents.push_back(CCoffin::Reset);
	listOfEvents.push_back(CCoffin::Shutdown);
	
	// Just raise the skeleton
	m_pSkeletonDrivePin->SetState(true);
	unsigned int  dwDelay = m_ApplicationSettings->GetInteger("COFFIN","SKELETONRISETME", m_skiSkeletonRiseTime);
	

	Event event = GetEvent(listOfEvents,dwDelay);
	
	
	ECoffinState eNextState = eSkeletonRaisedState;
	if(event != CCoffin::None)
	{
		switch(event)
		{
			case	CCoffin::Shutdown:		eNextState = eShutdown;			// Shutdown event
							break;

			case	CCoffin::Reset:		// Reset event
							eNextState = eResetSkeletonState;
							break;

		}
	}
	return eNextState;

}


CCoffin::ECoffinState CCoffin::SkeletonRaisedState()
{
	// Nothing to do but wait for the reset or shutdown
	std::list<CCoffin::Event> listOfEvents;
	listOfEvents.push_front(CCoffin::Reset);
	listOfEvents.push_front(CCoffin::Shutdown);


	CCoffin::Event event = GetEvent(listOfEvents,0);

	ECoffinState eNextState = eSkeletonRaisedState;	// Remain in this state for unknown events
	if(event != CCoffin::None)
	{
		switch(event)
		{
	
		case	CCoffin::Reset:	eNextState =  eResetSkeletonState;		// reset
			break;

		case	CCoffin::Shutdown:	eNextState = eShutdown;			// Shutdown event
			break;

		}
	}
	else
	{
		eNextState = eResetSkeletonState;
	}
	return eNextState;
}




int CCoffin::Run()
{

	bool	bExit = false;
	while(!bExit)
	{
		switch(m_eState)
		{
		case	eResetSkeletonState:		{
												CProjectLogger::GetInstance()->Log("Reset Skeleton State\n");
												m_eState = ResetSkeletonState();
											}
											break;

		case	eResetLidState:				{
												CProjectLogger::GetInstance()->Log("Reset Lid State\n");
												m_eState = ResetLidState();
											}
											break;

		case	ePIRResetState:				{
												CProjectLogger::GetInstance()->Log("PIR reset state\n");
												m_eState = PIRResetState();
											}
											break;

		case	eIdleState:					{
												CProjectLogger::GetInstance()->Log("Idle State\n");
												m_eState = IdleState();
											}
											break;


		case	eCoffinLidOpeningPlaySampleState:{
												CProjectLogger::GetInstance()->Log("Coffin Lid Opening Play Sample State\n");
												m_eState = CoffinLidOpeningPlaySampleState();
											}
											break;

		case	eCoffinLidOpeningState:		{
												CProjectLogger::GetInstance()->Log("Coffin Lid Opening State\n");
												m_eState = CoffinLidOpeningState();
											}
											break;


		case	eCoffinLidOpenState:		{
												CProjectLogger::GetInstance()->Log("Coffin Lid Open State\n");
												m_eState = CoffinLidOpenState();
											}
											break;

		case	eRaisingSkeletonState:		{
												CProjectLogger::GetInstance()->Log("Raising Skeleton State\n");
												m_eState = SkeletonRaisingState();
											}
											break;

		case	eSkeletonRaisedState:		{
												CProjectLogger::GetInstance()->Log("Skeleton Raised State\n");
												m_eState = SkeletonRaisedState();
											}
											break;

		
		case	eShutdown:					{
												bExit = true;
											}
											break;

		}
	}
	return 0;
}
