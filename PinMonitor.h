//
// Input pin
//
#ifndef __PINMONITOR_H
#define __PINMONITOR_H



#include "InPin.h"
#include "Listener.h"

#include <list>
#include <mutex>
#include "MonitoredPin.h"
#include "Thread.h"

class CMonitoredPin;


class CPinMonitor : public CThread
{
public:
	CPinMonitor();
	virtual ~CPinMonitor();
	static CPinMonitor* GetInstance();
	void AddPin(CMonitoredPin *pClient);
	void RemovePin(CMonitoredPin *pPin);
	int Run();
protected:
	typedef std::list<CMonitoredPin*> PinsList;
	PinsList	m_listPins;
	static CPinMonitor*	m_pThis;
	std::mutex m_mutex;
};


#endif //__PINMONITOR_H
