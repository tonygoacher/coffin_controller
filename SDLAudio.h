#ifndef __SDLMIXER_H
#define __SDLMIXER_H


#include "SDL_mixer.h"


#include <map>
#include <mutex>



class CAudioClient
{
public:
	virtual void SampleFinished() = 0;
};

class CSDLAudio
{
public:

	~CSDLAudio();
	
	static CSDLAudio* GetInstance();

	int PlaySample(const char *kpcPath, CAudioClient* pClient = NULL, int iLoops = 0);	// Loops -1 = continuous, 0 = once
	void SampleCallback(int iChannel);
	void StopPlayback(int iChannel);
protected:
	CSDLAudio();
	static CSDLAudio* m_pThis;
	typedef std::map<int, CAudioClient*> ClientMap_t;
	typedef std::pair<int, CAudioClient*> ClientPair_t;
	// A map of listeners indexed by channel
	ClientMap_t	m_mapClients;

	// A map of sample handles index by filename
	typedef std::map<std::string,  Mix_Chunk*> SampleHandleMap_t;
	typedef std::pair<std::string, Mix_Chunk*> SampleHandlePair_t;
	SampleHandleMap_t m_mapSampleHandles;

	std::mutex		m_SampleListMutex;
	std::mutex		m_ClientListMutex;
	
	int			m_iCurrentChannel;
};


#endif //__SDLMIXER_H
