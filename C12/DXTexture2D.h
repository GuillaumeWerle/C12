#pragma once

#include "DXDescriptorHandle.h"
#include "DXUploadable.h"
#include "FileSystem.h"
#include "DXSRV.h"

namespace DirectX
{
	class ScratchImage;
}

class DXBuffer;

class DXTexture2D : public DXUploadable
{
public:
	const DXSRV & GetSRV() const { return m_srv; }

	void LoadDDS(const FileSystem::Path & path);
	//void CreateDepthStencil(u32 width, u32 height);

	DXTexture2D();
	virtual ~DXTexture2D();

	virtual void Upload(DXRenderContext * rc) override;

private:
	UINT64 m_footPrintTotalBytes = 0;
	D3D12_RESOURCE_DESC m_desc;
	ComPtr<ID3D12Resource> m_resource;
	DXBuffer * m_uploadBuffer = nullptr;
	DXSRV m_srv;
	std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT> m_footPrintLayouts;
	std::vector<u32> m_numRows;
	std::vector<UINT64> m_rowSizeInBytes;

	void Create(const D3D12_RESOURCE_DESC & rdesc);
	void CopyScratchImageToUploadBuffer(DirectX::ScratchImage &scratch);
	void CreateUploadBuffer();
};

