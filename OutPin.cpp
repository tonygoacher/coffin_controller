

#include <pigpiod_if2.h>
#include <iostream>
#include "OutPin.h"
#include "pig.h"




COutPin::COutPin( int pin, bool bDefaultState)
{
	m_bState = bDefaultState;

	m_pin = pin;
	SetState(bDefaultState);
	int result = set_mode(PIGPIO::Instance()->GetPig(), pin,PI_OUTPUT);
	if(result < 0)
	{
		printf("Failed to init OutPin %d\n", pin);
		exit(0);
	}
}



COutPin::~COutPin()
{

}

bool COutPin::GetState()
{
	return m_bState;
}

void COutPin::SetState(bool bNewState)
{
	m_bState = bNewState;
	PIGPIO::Instance()->Write(m_pin,bNewState);

}
