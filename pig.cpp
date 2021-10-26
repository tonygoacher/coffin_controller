
#include <pigpiod_if2.h>
#include <stdio.h>
#include <stdlib.h>
#include "pig.h"


PIGPIO* PIGPIO::m_Instance = NULL;


PIGPIO::PIGPIO()
{
	m_Pig = pigpio_start(NULL,NULL);
	if(m_Pig < 0)
	{
		printf("Failed to init PIGPIO\n");
		exit(0);
	}
}

PIGPIO* PIGPIO::Instance()
{
	if(m_Instance == NULL)
	{
		m_Instance = new PIGPIO();
	}
	return m_Instance;
}


int PIGPIO::GetPig()
{
		return m_Pig;
}	

bool PIGPIO::Read(int port)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	return gpio_read(m_Pig, port);
}

void PIGPIO::Write(int port, bool state)
{
	std::lock_guard<std::mutex> lock(m_Mutex);	
	gpio_write(m_Pig, port, state);
}
