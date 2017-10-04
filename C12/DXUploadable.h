#pragma once

class DXRenderContext;

class DXUploadable
{
public:
	virtual void Upload(DXRenderContext* rc) = 0;

protected:
	DXRenderContext* m_uploadRenderContext = nullptr;
	bool m_uploadRequested = false;
	bool m_uploadDone = false;
};

