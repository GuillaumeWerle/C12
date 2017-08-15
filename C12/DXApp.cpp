#include "stdafx.h"
#include "DXApp.h"
#include "DXRenderer.h"



DXApp::DXApp()
	: m_renderer(new DXRenderer())
{
}


DXApp::~DXApp()
{
}

void DXApp::Init()
{
	m_renderer->Init();
}
