// A loger to change text output colour according to CCoffin state
#ifndef __COFFIN_LOGGER_H
#define __COFFIN_LOGGER_H
#include "Logger.h"

class CCoffinModeLogger : public CLogger
{
public:
	CCoffinModeLogger(){};
	virtual ~CCoffinModeLogger(){};

	void Log(const char*fmt, va_list ap);



private:
	void SetTextColor();

};





#endif //__COFFIN_LOGGER_H

