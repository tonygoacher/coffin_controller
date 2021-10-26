

#include "PinMonitor.h"

#include "MonitoredPin.h"



CPinMonitor* CPinMonitor::m_pThis = NULL;





CPinMonitor::CPinMonitor()
{
	// Reset the listener list
	m_listPins.clear();
	Start();
}

CPinMonitor::~CPinMonitor()
{
	if(ThreadIsRunning())
	{
		Stop();
	}
	m_listPins.clear();
}

CPinMonitor* CPinMonitor::GetInstance()
{
	if(m_pThis == NULL)
	{
		m_pThis = new CPinMonitor();
	}
	return m_pThis;
}


void CPinMonitor::AddPin(CMonitoredPin *pPin)
{
	

	std::lock_guard<std::mutex> lock(m_mutex);

	pPin->LatchState();
	m_listPins.push_back(pPin);
	if(!ThreadIsRunning())
	{
		Start();
	}
}

void CPinMonitor::RemovePin(CMonitoredPin *pPin)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_listPins.remove(pPin);
}


int CPinMonitor::Run()
{
	while(!ShouldExit(10))
	{
		m_mutex.lock();
		for(PinsList::iterator i = m_listPins.begin(); i != m_listPins.end() ; i++)
		{
			CMonitoredPin *pPin = (CMonitoredPin*)(*i);
			pPin->CheckPinChanged();
		}
		m_mutex.unlock();
	};
	return 0;
}
