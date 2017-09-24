#pragma once

#include "DXDescriptorHandle.h"
#include "DXUploadable.h"

class DXBuffer;

class DXTexture2D : public DXUploadable
{
public:
	UINT64 m_footPrintTotalBytes = 0;
	D3D12_RESOURCE_DESC m_desc;
	ComPtr<ID3D12Resource> m_resource;
	DXBuffer * m_uploadBuffer = nullptr;
	DXDescriptorHandle m_srv;
	std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT> m_footPrintLayouts;
	std::vector<u32> m_numRows;
	std::vector<UINT64> m_rowSizeInBytes;


	void Init();

	void CreateFromDesc(const D3D12_RESOURCE_DESC & rdesc);

	virtual void Upload(DXRenderContext * rc) override;

	DXTexture2D();
	~DXTexture2D();
};

