#pragma once

#include "Thread.h"

class DXApp;

class MasterRenderThread : public Thread
{
public:
	DXApp * m_app;

	virtual void StartThread() override;
	virtual void EndThread() override;

	virtual bool ThreadLoop() override;

	MasterRenderThread(DXApp * app);
	~MasterRenderThread();



};

