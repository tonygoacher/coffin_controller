

#include "CoffinLogger.h"
#include "Coffin.h"

void CCoffinModeLogger::Log(const char*fmt,va_list ap)
{
	char rgcBuff[1024];
	memset(rgcBuff, 0, sizeof(rgcBuff));
	
	vsnprintf( rgcBuff, sizeof(rgcBuff),  fmt, ap);
	SetTextColor();
	printf(rgcBuff);
}


void CCoffinModeLogger::SetTextColor()
{
	
		
}
