#pragma once

class DXRenderContext;

class DXUploadable
{
public:

	DXRenderContext * m_uploadRenderContext = nullptr;
	bool m_uploadRequested = false;
	bool m_uploadDone = false;

	virtual void Upload(DXRenderContext * rc) = 0;

};

