#pragma once

#include "SDLAudio.h"
#include "INIReader.h"


class CSoundList : public CAudioClient
{
public:

	virtual ~CSoundList();
	void SampleFinished();
	void Go() {NextSound();}
	void SetVolume(int iVol);
	static CSoundList* GetInstance();
private:
	static CSoundList* m_pThis;
	void NextSound();
	CSoundList();
	INIReader*	m_pAppSettings;
	int	m_iIndex;
	int		m_iCurrentChannel;
	bool	m_bShutDown;


};
