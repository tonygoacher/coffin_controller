#ifndef __LOGGER_H
#define __LOGGER_H

#include <stdarg.h>


class CLogger
{
public:
	virtual void Log(const char*fmt,va_list ap) = 0;
};


#endif //__LOGGER_H

