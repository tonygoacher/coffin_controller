#ifndef __LISTENER_H
#define __LISTENER_H



#include <mutex>
#include <map>

// A generic listener class

template <class T> 
class CListener
{
	protected:
	std::mutex m_Mutex;
	typedef	std::map<T *, int> ListenerCollection_t;
//	typedef	ListenerCollection_t::iterator ListenerCollectionIterator_t;
	typedef std::pair<T*, int> ListenerPair_t;

	ListenerCollection_t	m_ListenerList;
	
public:
	CListener()
	{
	
	}

	std::mutex &GetMutex() 
	{
		return m_Mutex;
	}

	void AddListener(T *pListener)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);

		typename CListener::ListenerCollection_t::iterator i = m_ListenerList.find(pListener) ;
		// If already in the list, increment the iterator
		if(i != m_ListenerList.end())
			(*i).second++ ;
		else
			m_ListenerList.insert(ListenerPair_t(pListener,1)) ;

	}

	void RemoveListener(T * pListener)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);

		typename CListener::ListenerCollection_t::iterator i = m_ListenerList.find(pListener) ;
		// If found, decrement ref count
		if(i != m_ListenerList.end())
		{
			if((*i).second)
				((*i).second)-- ;
		}

	}

	// 
	void ListenerTidy()
	{
		std::lock_guard<std::mutex> lock(m_Mutex);

		for(typename CListener::ListenerCollection_t::iterator i=m_ListenerList.begin(); i != m_ListenerList.end();)
		{
			// Check the ref count is none 0
			if((*i).second==0)
			{
				// Ref count 0, remove it
				i = m_ListenerList.erase(i) ;
			}
			else
				i++;

		}
	}



};


#endif //__LISTENER_H
