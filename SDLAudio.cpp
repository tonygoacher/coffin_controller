

#include <unistd.h>
#include "SDLAudio.h"
#include <string>
#include <errno.h>
#include "ProjectLogger.h"



CSDLAudio* CSDLAudio::m_pThis = NULL;


void SampleHasFinished(int iChannel)
{
	CSDLAudio::GetInstance()->SampleCallback(iChannel);
}

CSDLAudio* CSDLAudio::GetInstance()
{
	if(m_pThis == NULL)
	{
		m_pThis = new CSDLAudio();
	}
	return m_pThis;
}

CSDLAudio::CSDLAudio()
{
	m_mapClients.clear();

	Mix_ChannelFinished(SampleHasFinished);
}

CSDLAudio::~CSDLAudio()
{
	m_ClientListMutex.lock();
	m_mapClients.clear();
	m_ClientListMutex.unlock();
	Mix_CloseAudio();
}


int CSDLAudio::PlaySample(const char *kpcPath, CAudioClient* pClient /* = NULL */, int iLoops /* = 0 */)
{

	std::lock_guard<std::mutex> lock(m_SampleListMutex);
	std:: string filename = kpcPath;
	std::string filePath = std::string("resources/") + filename;

	Mix_Chunk *SampleMixChunk;
	// See if this sample has already been loaded
	
	SampleHandleMap_t::iterator sampleHandleIter = m_mapSampleHandles.find(filePath);


	if(sampleHandleIter != m_mapSampleHandles.end())
	{
		CProjectLogger::GetInstance()->Log("Found %s\n",filePath.c_str());
		// The sample is already loaded, just play it
		SampleMixChunk = sampleHandleIter->second;
	}
	else
	{
		
		if(access(filePath.c_str(), F_OK) == -1)
		{
			printf("Failed to open sample for playing: %s\n", filePath.c_str());
			exit(0);
		}
		
		CProjectLogger::GetInstance()->Log("Loading %s\n",filePath.c_str());
		// This is a new sample, load it
		SampleMixChunk = Mix_LoadWAV(filePath.c_str());
	
		
		// Now add this path and handle to the map
		m_mapSampleHandles.insert(SampleHandlePair_t(filePath.c_str(), SampleMixChunk));

	}
 	// Now play the sample and get a channel for it
	CProjectLogger::GetInstance()->Log("Playing %s\n",filePath.c_str());
 	int iChannel = Mix_PlayChannel(-1, SampleMixChunk, iLoops);
 


 	// And add it and the client to the client map
 	m_ClientListMutex.lock();
 	m_mapClients.insert(ClientPair_t(iChannel, pClient));
 	m_ClientListMutex.unlock();

	return iChannel;

}




void CSDLAudio::SampleCallback(int iChannel)
{
	m_ClientListMutex.lock();
	ClientMap_t::iterator iter = m_mapClients.find(iChannel);
	if(iter != m_mapClients.end())
	{
		CAudioClient *pClient = iter->second;
		m_mapClients.erase(iter);
		if(pClient)
		{
			m_ClientListMutex.unlock();
			pClient->SampleFinished();
			return;
		}

	}
	m_ClientListMutex.unlock();
}


void CSDLAudio::StopPlayback(int iChannel)
{
	m_ClientListMutex.lock();
	ClientMap_t::iterator iter = m_mapClients.find(iChannel);
	if(iter != m_mapClients.end())
	{
		iter->second = NULL;
	}
	Mix_HaltChannel(iChannel);
	m_ClientListMutex.unlock();
}
