


#include "SDLWrapper.h"

#include "ProjectLogger.h"


CSDLWrapper* CSDLWrapper::m_pThis = NULL;

CSDLWrapper* CSDLWrapper::GetInstance()
{
	if(m_pThis == NULL)
	{
		m_pThis = new CSDLWrapper();
	}
	return m_pThis;
}


CSDLWrapper::CSDLWrapper()
{

}

void CSDLWrapper::Init(eInitType eType)
{
	m_eInitType = eType;
	if(m_eInitType == eAudioOnly)
	{
		SDL_Init(SDL_INIT_AUDIO);
	}

	if(m_eInitType == eAudioOnly)
	{
		  /* Same setup as before */
		 int audio_rate = 22050;
		 Uint16 audio_format = AUDIO_S16; 
		 int audio_channels = 2;
		 int audio_buffers = 4096;

		 if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
		 {
			 CProjectLogger::GetInstance()->Log("Cannot open audio");
			 exit(1);
		 }
	}
}


CSDLWrapper::~CSDLWrapper()
{
	if(m_eInitType == eAudioOnly)
	{
		Mix_CloseAudio();
	}
	SDL_Quit();
}
