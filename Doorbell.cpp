


#include "Doorbell.h"
#include "PinMonitor.h"
#include "inifile.h"
#include "SoundList.h"


CDoorBell* CDoorBell::m_pThis = NULL;

CDoorBell* CDoorBell::GetInstance()
{
	if(m_pThis == NULL)
	{
		m_pThis = new CDoorBell();
	}

	return m_pThis;
}

CDoorBell::CDoorBell()
{
	m_ApplicationSettings = new INIReader(INIPATH);
	
	int pin = m_ApplicationSettings->GetInteger("PINS","DOORBELL", 25);
	m_pPin = new CMonitoredPin( pin, true);
	// Add this class as a client
	m_pPin->AddClient(this);

		
	pin = m_ApplicationSettings->GetInteger("PINS","STROBE", 18);
	bool bPinDefault = m_ApplicationSettings->GetInteger("PINS","STROBEDEFAULT", false) != 0;
	m_pTimedOutput = new CTimedOutput( pin, bPinDefault);
	CPinMonitor::GetInstance()->AddPin(m_pPin);
	m_SamplePlaying = false;
	sem_init(&m_sem, 0, 0);
	Start();

}


CDoorBell::~CDoorBell()
{

	if(ThreadIsRunning())
	{	
		Stop(&m_sem);
	}
	CPinMonitor::GetInstance()->RemovePin(m_pPin);
	delete m_pTimedOutput;
}

void CDoorBell::PinChanged(CMonitoredPin* pPin)
{
	if(pPin->GetState() && !m_SamplePlaying)
	{
		printf("Doorbell seen\n");
		sem_post(&m_sem);
	}
}

void CDoorBell::StopEvent()
{
	sem_post(&m_sem);	// unblock thread so it can exit
}

int CDoorBell::Run()
{
	while(!ShouldExit(0))
	{
		sem_wait(&m_sem);
		
		if(!ShouldExit())
		{
			m_SamplePlaying = true;
			m_pTimedOutput->Fire();		// Start the strobe
			std::string sample;
			sample = m_ApplicationSettings->GetString(std::string(DOORBELL_SEC)
				,std::string(DOORBELLSAMPLE_KEY),"");
		
			m_BlockedSample.PlaySample(sample.c_str());
	

			// If exit requested during sample playback
			if(ShouldExit())
			{
				return 0;
			}
			m_SamplePlaying = false;
			
		}
	}
	return 0;
}

void CDoorBell::PlayBlocked()
{

		CSoundList::GetInstance()->SetVolume(0);
		
		m_BlockedSample.PlaySample("thunder.wav");

		CSoundList::GetInstance()->SetVolume(128);
}
