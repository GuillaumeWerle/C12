#pragma once

#include "DXDescriptorHeap.h"
#include "DXUploadable.h"

class DXTexture2D : public DXUploadable
{
public:
	D3D12_RESOURCE_DESC m_desc;
	ComPtr<ID3D12Resource> m_resource;
	ComPtr<ID3D12Resource> m_uploadBuffer;
	DXDescriptorHandle m_srv;

	void Init();
	virtual void Upload(DXRenderContext * rc) override;

	DXTexture2D();
	~DXTexture2D();
};

