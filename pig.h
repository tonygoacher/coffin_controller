#ifndef __PIGPIO_H
#define __PIGPIO_H

#include <mutex>


class PIGPIO
{
public:

	static PIGPIO* Instance() ;
	void Stop();
	int GetPig();
	bool Read(int port);
	void Write(int port, bool state);
private:
	std::mutex		m_Mutex;
	PIGPIO();
	int 	m_Pig;
	static PIGPIO* m_Instance;
	
};




#endif //__PIGPIO_H
