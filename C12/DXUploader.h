#pragma once

class DXRenderContext;
class DXUploadable;

class DXUploader
{
public:

	void RequestUpload(DXUploadable * up);
	void ExecuteUploadRequests(DXRenderContext * rc);

	DXUploader();
	~DXUploader();


private:

	std::mutex m_mutex;
	std::vector<DXUploadable*> m_requests;

};

