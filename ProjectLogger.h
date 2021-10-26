#ifndef __PROJECT_LOGGER_H
#define __PROJECT_LOGGER_H

#include "Logger.h"
#include "CoffinLogger.h"

class CProjectLogger
{
public:
	virtual ~CProjectLogger();
	static CProjectLogger* GetInstance();
	void Log(const char*fmt,...);
private:
	CProjectLogger();

	CLogger*	m_pLogger;
	static CProjectLogger* m_pThis;
};

#endif // __PROJECT_LOGGER_H