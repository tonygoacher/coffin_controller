#ifndef __COFFIN_H
#define __COFFIN_H

#include <mutex>
#include "PinMonitor.h"
#include "OutPin.h"

#include "SDLAudio.h"
#include "INIReader.h"
#include <list>
#include "inifile.h"

class CMonitoredPin;


class CCoffin : public CMonitoredPinClient, CThread, CAudioClient
{
public:

	enum EMode{eGreenMode, eAmberMode, eRedMode, eNumModes};

	virtual ~CCoffin();

	int Run();
	static CCoffin* GetInstance();
	void SampleFinished();

	void ModeChange();
	CCoffin::EMode GetMode()  { return m_eMode;}

protected:
	void PinChanged(CMonitoredPin* pPin);

	void StateMachine();



private:
	CCoffin();
	enum ECoffinState {eResetSkeletonState, eResetLidState, ePIRResetState,eIdleState, eCoffinLidOpeningState, eCoffinLidOpeningPlaySampleState,eCoffinLidOpenState,eRaisingSkeletonState,eSkeletonRaisedState, eShutdown};


	ECoffinState ResetSkeletonState();
	ECoffinState ResetLidState();
	ECoffinState IdleState();
	ECoffinState CoffinLidOpeningState();
	ECoffinState CoffinLidOpenState();
	ECoffinState SkeletonRaisingState();
	ECoffinState SkeletonRaisedState();
	ECoffinState CoffinLidOpeningPlaySampleState();
	ECoffinState PIRResetState();

	void ChangeDemoMode(CInPin *pPin);

	ECoffinState	m_eState;
	INIReader*	m_ApplicationSettings;

	
	typedef enum
	{
		None,
		PIR,
		Reset,
		Doorbell,
		Shutdown,
		Demo,
	}Event;	
	
	Event m_Event;
	
	Event GetEvent(std::list<CCoffin::Event> events, int delay = 0);

	CMonitoredPin*		m_pResetPin;
	CMonitoredPin*		m_pPIRPin;

	COutPin*	m_pSkeletonDrivePin;
	COutPin*	m_pCoffinLidDrivePin;

	std::mutex		m_LedMutex;
	static		std::mutex		m_InstanceMutex;
	static CCoffin*	m_pThis;

	EMode		m_eMode;
	bool		m_bDemoMode;

	static const int	m_skiSkelResetDelay = 16;
	static const int	m_skiLidResetDelay = 6;
	static const int	m_skiLidRiseTime = 2;
	static const int	m_skiSkeletonRiseTime = 4;
	static const int	m_skiLidOpenTimeout	=	60;	// 5 minutes
	static const int	m_skiRaisedSkeletonTimeout = 300;
};



#endif
