#include "SDL.h"
#include "SDL_mixer.h"


class CSDLWrapper
{
public:
	enum eInitType
	{
		eAudioOnly,
	};
	CSDLWrapper();
	~CSDLWrapper();
	void Init(eInitType eType);
	static CSDLWrapper* GetInstance();
protected:
	static CSDLWrapper*	m_pThis;
	eInitType	m_eInitType;

};