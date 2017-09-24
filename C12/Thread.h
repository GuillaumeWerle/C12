#pragma once

class Thread
{
public:

	virtual void StartThread();
	virtual void EndThread();

	virtual bool ThreadLoop() = 0;

	Thread();
	virtual ~Thread();

protected:
	void InternalLoop();

private:
	std::thread * m_thread;	
	bool m_running = false;
};

