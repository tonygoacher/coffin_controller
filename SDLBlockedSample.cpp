

#include <stdio.h>
#include "SDLBlockedSample.h"
#include "SDLAudio.h"



CSDLBlockedSample::CSDLBlockedSample()
{
	sem_init(&m_sem, 0,0);	// Default to blocked
}

void CSDLBlockedSample::SampleFinished()
{
	sem_post(&m_sem);
}


void CSDLBlockedSample::PlaySample(const char *kpcSample)
{
	CSDLAudio::GetInstance()->PlaySample(kpcSample, this);
	sem_wait(&m_sem);
}
