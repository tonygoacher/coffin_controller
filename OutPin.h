//
// Output pin
//
#ifndef __OUTPIN_H
#define __OUTPIN_H




class COutPin
{
public:
	COutPin(int pin, bool bDefaultState);
	virtual ~COutPin();
	bool GetState();
	void SetState(bool bNewState);
protected:
	
	int  m_pin;
	bool m_bState;
};


#endif //__OUTPIN_H
