#include "stdafx.h"
#include "MasterRenderThread.h"
#include "DXApp.h"

MasterRenderThread::MasterRenderThread(DXApp * app)
{
	m_app = app;
}

MasterRenderThread::~MasterRenderThread()
{
}

void MasterRenderThread::StartThread()
{
	Thread::StartThread();
}


void MasterRenderThread::EndThread()
{
	Thread::EndThread();
}

bool MasterRenderThread::ThreadLoop()
{
	m_app->Update();
	m_app->Render();

	return true;
}