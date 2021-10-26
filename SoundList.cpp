


#include <unistd.h>
#include "SoundList.h"
#include "SDLAudio.h"
#include "inifile.h"

#define _MAX_PATH 256


CSoundList* CSoundList::m_pThis = NULL;

CSoundList* CSoundList::GetInstance()
{
	if(m_pThis == NULL)
	{
		m_pThis = new CSoundList();
	}
	return m_pThis;
}

CSoundList::CSoundList()
{
	m_pAppSettings = new INIReader(INIPATH);
	m_iIndex = -1;
	m_bShutDown = false;
	
}


CSoundList::~CSoundList()
{
	m_bShutDown = true;
	CSDLAudio::GetInstance()->StopPlayback(-1);
	delete m_pAppSettings;
}

void CSoundList::SampleFinished()
{
	printf("soundList sample finished\n");
	if(!m_bShutDown)
	{
		NextSound();
	}
}

void CSoundList::SetVolume(int iVol)
{
	if(m_iIndex != -1)
	{
		Mix_Volume(m_iCurrentChannel, iVol);
	}
}

void CSoundList::NextSound()
{
	
	std::string sound;
	m_iIndex++;
	do 
	{
		char rgcSoundKey[128];
		sprintf(rgcSoundKey,BACKGROUNDSAMPLE_KEY,m_iIndex);

		sound = m_pAppSettings->Get(std::string(BACKGROUNDSAMPLE_SEC),std::string(rgcSoundKey),std::string(""));
		if(sound.length() == 0)
		{
			m_iIndex = 0;
		}
	} while(!sound.length());
	if(access(sound.c_str(), F_OK) == -1)
	{
		printf("Failed to open sample %s\n", sound.c_str());
	}
	printf("Playing background sample %s\n",sound.c_str());
	m_iCurrentChannel = CSDLAudio::GetInstance()->PlaySample(sound.c_str(), this);
}
