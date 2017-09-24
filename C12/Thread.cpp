#include "stdafx.h"
#include "Thread.h"


Thread::Thread()
{
	m_thread = nullptr;
}


Thread::~Thread()
{
	assert(m_running == false);	// EndThread() was not called before destructor.
}

void Thread::InternalLoop()
{
	while (m_running)
	{
		bool ret = ThreadLoop();
		if (ret == false)
			m_running = false;
	}
}

void Thread::StartThread()
{
	m_running = true;
	m_thread = new std::thread(&Thread::InternalLoop, this);
}

void Thread::EndThread()
{
	m_running = false;
	m_thread->join();
	delete m_thread;
	m_thread = nullptr;
}

