


#include "MonitoredPin.h"


CMonitoredPin::CMonitoredPin(int pin, bool bInverted /* = false */)
:CInPin( pin, bInverted)
{
	m_listClients.clear();
	LatchState();
}

CMonitoredPin::~CMonitoredPin()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_listClients.clear();
}

void CMonitoredPin::LatchState()
{
 
	m_bState = GetState();
}

bool CMonitoredPin::StateChanged()
{ 
	bool bNow = GetState();
	return m_bState != bNow;
}


void CMonitoredPin::AddClient(CMonitoredPinClient* pClient)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_listClients.push_back(pClient);
}

void CMonitoredPin::RemoveClient(CMonitoredPinClient* pClient)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_listClients.remove(pClient);
}


void CMonitoredPin::CheckPinChanged()
{
	if(StateChanged())
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		MonitoredPinClientList::iterator iterClients = m_listClients.begin();
		while(iterClients != m_listClients.end())
		{
			CMonitoredPinClient* pClient = (CMonitoredPinClient*)(*iterClients);
			pClient->PinChanged(this);
			iterClients++;
		}
		LatchState();
	}
}
