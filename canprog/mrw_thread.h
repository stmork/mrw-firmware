/*
**
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Multithreading control
**
**	(C) Copyright 2010  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef MRW_THREAD_H
#define MRW_THREAD_H

#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>

#ifndef null
#define null 0L
#endif

typedef unsigned int (*ThreadProc)(void *);

class ThreadInfo
{
protected:
	inline static bool CheckResult(int error_code)
	{
#ifdef _DEBUG
		if (error_code != 0)
		{
			fprintf(stderr, "### pthread # errno: %d (%s)!\n",
					 error_code,strerror(error_code));
			fflush(stderr);
		}
#endif
		return error_code == 0;
	}
};

/**
 * This class defines a critical sections where
 * only one thread can use a resource. This class uses
 * POSIX threads semantics for multi threading inside
 * one process. Note that this class cannot be used
 * as global variables.
 */
class Mutex : protected ThreadInfo
{
	pthread_mutex_t mutex;

public:
	/**
	 * This constructor initializes the mutex.
	 */
	Mutex()
	{
		CheckResult(pthread_mutex_init(&mutex,NULL));
	}

	/**
	 * Ths destructor deinitializes the mutex.
	 */
	virtual ~Mutex()
	{
		CheckResult(pthread_mutex_destroy(&mutex));
	}

	inline bool  Lock()
	{
		return CheckResult(pthread_mutex_lock(&mutex));
	}

	inline bool  Unlock()
	{
		return CheckResult(pthread_mutex_unlock(&mutex));
	}
};

class Lock
{
	Mutex &m_Mutex;

public:
	inline Lock(Mutex &mutex) : m_Mutex(mutex)
	{
		m_Mutex.Lock();
	}

	inline ~Lock()
	{
		m_Mutex.Unlock();
	}
};

/**
 * This class implements a simple signalling event pipe..
 */
class Event : protected ThreadInfo
{
	pthread_cond_t    event;
	pthread_mutex_t   mutex;
	volatile bool     pulse;

public:
	/**
	 * This constructor initializes this event handler.
	 */
	inline Event()
	{
		CheckResult(pthread_cond_init(&event,NULL));
		CheckResult(pthread_mutex_init(&mutex,NULL));
		pulse = false;
	}

	/**
	 * This destructor deinitializes this event handler.
	 */
	inline virtual ~Event()
	{
		CheckResult(pthread_mutex_destroy(&mutex));
		CheckResult(pthread_cond_destroy(&event));
	}

	inline void     Pulse() 
	{
		CheckResult(pthread_mutex_lock(&mutex));
		pulse = true;
		CheckResult(pthread_cond_broadcast(&event));
		CheckResult(pthread_mutex_unlock(&mutex));
	}

	inline bool     Wait()
	{
		bool success = true;

		success &= CheckResult(pthread_mutex_lock(&mutex));
		if (!pulse)
		{
			success &= CheckResult(pthread_cond_wait(&event,&mutex));
		}
		pulse = false;
		success &= CheckResult(pthread_mutex_unlock(&mutex));

		return success;
	}
};

/**
 * This class implements threading.
 */
class Thread : protected ThreadInfo
{
	const char              *m_Name;
	int                      m_Prio;

	pthread_t                m_Thread;
	volatile bool            m_IsRunning;
	volatile unsigned int    m_Result;
	volatile ThreadProc      m_CallProc;
	volatile void           *m_CallArg;

public:
	/**
	 * This constructor initializes this instance and can name the thread.
	 *
	 * @param taskname The new thread name.
	 */
	inline Thread(const char *taskname = null)
	{
		m_Name      = taskname;
		m_IsRunning = false;
		m_Result    = 0;
		m_Thread    = 0;
	}

	/**
	 * This destructor terminates a running thread and deinitializes this instance.
	 */
	inline virtual ~Thread()
	{
		Stop();
	}

	inline void Name(const char *taskname = null)
	{
		m_Name = taskname;
	}

	inline bool Start(ThreadProc proc, void *ptr, int priority=0)
	{
		bool  success;
		int   error_code;

		Stop();
		assert(!m_IsRunning);

		m_CallProc = proc;
		m_CallArg  = ptr;
		m_Result   = 0;
		m_Thread   = 0;
		m_Prio     = -priority * 5;

		return CheckResult(error_code = pthread_create(&m_Thread,NULL,&Trampoline,this));
	}

	inline bool          IsRunning()
	{
		return m_IsRunning;
	}

	inline bool          Stop()
	{
		bool was_running;

		if (IsRunning())
		{
			pthread_cancel(m_Thread);
		}
		was_running = m_IsRunning;

		m_CallProc  = null;
		m_CallArg   = 0;
		m_Thread    = 0;

		return was_running;
	}

	inline unsigned int  Wait()
	{
		int   result;
		void *ptr = &result;

		CheckResult(pthread_join(m_Thread,&ptr));
		return m_Result;
	}

private:
	static void *Trampoline(void *ptr)
	{
		Thread *threadClass = (Thread *)ptr;

		nice(threadClass->m_Prio);
		threadClass->m_Result = threadClass->m_CallProc((void *)threadClass->m_CallArg);

		return null;
	}
};

#endif
