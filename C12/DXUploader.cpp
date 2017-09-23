#include "stdafx.h"
#include "DXUploader.h"
#include "DXUploadable.h"

DXUploader::DXUploader()
{
}


DXUploader::~DXUploader()
{
}

void DXUploader::RequestUpload(DXUploadable * up)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	m_requests.push_back(up);
}

void DXUploader::ExecuteUploadRequests(DXRenderContext * rc)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	for (auto it : m_requests)
		it->Upload(rc);

	m_requests.clear();
}

