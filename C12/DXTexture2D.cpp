#include "stdafx.h"
#include "DXTexture2D.h"
#include "DX.h"
#include "DXHelpers.h"
#include "DXRenderContext.h"
#include "DXBuffer.h"


DXTexture2D::DXTexture2D()
{
}

DXTexture2D::~DXTexture2D()
{
	delete m_uploadBuffer;
	DX::PoolSRVCBVUAV->Free(m_srv);
}

void DXTexture2D::Init()
{
	D3D12_RESOURCE_DESC rdesc = {};
	rdesc.MipLevels = 1;
	rdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rdesc.Width = 256;
	rdesc.Height = 256;
	rdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	rdesc.DepthOrArraySize = 1;
	rdesc.SampleDesc.Count = 1;
	rdesc.SampleDesc.Quality = 0;
	rdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	rdesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	m_desc = rdesc;

	DX::Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&rdesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_resource));

	m_srv = DX::PoolSRVCBVUAV->Alloc();
	DX::Device->CreateShaderResourceView(m_resource.Get(), nullptr, m_srv.CPU);

	UINT64 footPrintTotalBytes = 0;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footPrintLayout;
	UINT numRows;
	UINT64 rowSizeInBytes;
	DX::Device->GetCopyableFootprints(&rdesc, 0, 1, 0, &footPrintLayout, &numRows, &rowSizeInBytes, &footPrintTotalBytes);

	m_uploadBuffer = new DXBuffer;
	m_uploadBuffer->Init(D3D12_HEAP_TYPE_UPLOAD, footPrintTotalBytes);

	std::vector<u32> texture;
	texture.resize(256*256);

	u32 white = DXHelpers::RGBA(255, 255, 255, 255);
	u32 black = 0;

	for (u32 y = 0; y < 256; y++)
	{
		bool by = (y >> 2) & 1;
		for (u32 x = 0; x < 256; x++)
		{
			bool bx = (x >> 2) & 1;
			texture[x + y * 256] = bx == by ? white : black;
		}
	}

	u8 * cpuPtr = m_uploadBuffer->m_cpuPtr;
	for (u64 row = 0; row < numRows; row++)
	{
		memcpy(cpuPtr, &texture[row * 256], 256 * sizeof(u32));
		cpuPtr += rowSizeInBytes;
	}
}

void DXTexture2D::Upload(DXRenderContext * rc)
{
	UINT64 footPrintTotalBytes = 0;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT placedSubResourceFootprint;
	UINT rowCount;
	UINT64 rowPitchInByte;
	DX::Device->GetCopyableFootprints(&m_desc, 0, 1, 0, &placedSubResourceFootprint, &rowCount, &rowPitchInByte, &footPrintTotalBytes);

	CD3DX12_TEXTURE_COPY_LOCATION dst(m_resource.Get(), 0);
	CD3DX12_TEXTURE_COPY_LOCATION src(m_uploadBuffer->m_buffer.Get(), placedSubResourceFootprint);
	rc->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
	rc->ResourceBarrier(m_resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
}

