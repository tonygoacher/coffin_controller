#ifndef __SDLBLOCKEDSAMPLE_H
#define __SDLBLOCKEDSAMPLE_H

#include <semaphore.h>
#include "SDLAudio.h"



class CSDLBlockedSample : public CAudioClient
{
public:
	CSDLBlockedSample();
	~CSDLBlockedSample(){};
	virtual void SampleFinished();
	void PlaySample(const char *kpcSample);

protected:
	sem_t	m_sem;
};

#endif //__SDLBLOCKEDSAMPLE_H
