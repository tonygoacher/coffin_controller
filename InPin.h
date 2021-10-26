//
// Input pin
//
#ifndef __INPIN_H
#define __INPIN_H




class CInPin
{
public:
	CInPin(int ioPin, bool bInverted = false);
	virtual ~CInPin();
	bool GetState();
protected:
	
	int			m_pin;
	bool		m_bInverted;
};


#endif //__INPIN_H
