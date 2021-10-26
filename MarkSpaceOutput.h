#pragma  once

#include "OutPin.h"
#include "Thread.h"
#include "INIReader.h"



class CMarkSpaceOutput : public CThread, public COutPin
{
public:
	CMarkSpaceOutput(int pin, bool bDefault);
	~CMarkSpaceOutput();
	int Run();
private:
	INIReader *m_pAppSettings;
};
