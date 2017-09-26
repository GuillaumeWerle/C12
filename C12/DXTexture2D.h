#pragma once

#include "DXDescriptorHandle.h"
#include "DXUploadable.h"
#include "FileSystem.h"

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


	void LoadDDS(const FileSystem::Path & path);

	void Create(const D3D12_RESOURCE_DESC & rdesc);
	void CreateUploadBuffer();

	virtual void Upload(DXRenderContext * rc) override;

	DXTexture2D();
	~DXTexture2D();
};

