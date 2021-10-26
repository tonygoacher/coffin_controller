
#include <pigpiod_if2.h>
#include <iostream>
#include "InPin.h"
#include "pig.h"




CInPin::CInPin( int ioPin, bool bInverted)
{
	
	m_pin = ioPin;
	m_bInverted = bInverted;
	int result = set_mode(PIGPIO::Instance()->GetPig(), ioPin ,PI_INPUT);
	if(result < 0)
	{
		printf("Failed to init InPin %d\n", ioPin);
		exit(0);
	}
	result = set_pull_up_down(PIGPIO::Instance()->GetPig(), ioPin, PI_PUD_UP);

	if(result < 0)
	{
		printf("Failed to init InPin pull up %d\n", ioPin);
		exit(0);
	}	
}

CInPin::~CInPin()
{

}


bool CInPin::GetState()
{
	bool pinState = PIGPIO::Instance()->Read(m_pin) == 1;
	return (pinState != 0) ^ m_bInverted;
}
