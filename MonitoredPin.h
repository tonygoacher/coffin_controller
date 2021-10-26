//
//
//
#ifndef __MONITOREDPIN_H
#define __MONITOREDPIN_H

#include "InPin.h"
#include <list>
#include <mutex>



class CMonitoredPin;

class CMonitoredPinClient
{
public:
	virtual void PinChanged(CMonitoredPin* pChangedPin) = 0;
};

class CMonitoredPin : public CInPin
{
public:
	CMonitoredPin(int pin, bool bInverted = false);
	virtual ~CMonitoredPin();
	
	void AddClient(CMonitoredPinClient* pClient);
	
	void RemoveClient(CMonitoredPinClient* pClient);


	bool	StateChanged();
	void	LatchState();

	void	CheckPinChanged();
	

private:
	typedef std::list<CMonitoredPinClient*> MonitoredPinClientList;
	MonitoredPinClientList	m_listClients;
	std::mutex m_mutex;
	bool	m_bState;

};

#endif
