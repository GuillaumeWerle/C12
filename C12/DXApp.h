#pragma once

class DXRenderer;

class DXApp
{
public:

	uint32_t m_width = 1920;
	uint32_t m_height = 1080;
	
	std::unique_ptr<DXRenderer> m_renderer;

	void Init();

	DXApp();
	~DXApp();
};

