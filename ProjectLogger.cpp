

#include <stdarg.h>
#include "ProjectLogger.h"

CProjectLogger* CProjectLogger::m_pThis = nullptr;


CProjectLogger::CProjectLogger()
{
	m_pLogger = new CCoffinModeLogger();
}

CProjectLogger::~CProjectLogger()
{
	delete m_pLogger;
}

CProjectLogger* CProjectLogger::GetInstance()
{
	if(!m_pThis)
	{
		m_pThis = new CProjectLogger();
	}
	return m_pThis;
}

void CProjectLogger::Log(const char*fmt,...)
{
	va_list args;
	va_start(args, fmt);
	m_pLogger->Log(fmt, args);
}

