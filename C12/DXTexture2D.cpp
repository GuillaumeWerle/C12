#include "stdafx.h"
#include "DXTexture2D.h"
#include "DX.h"
#include "DXHelpers.h"


DXTexture2D::DXTexture2D()
{
}

DXTexture2D::~DXTexture2D()
{
	DX::PoolSRVCBVUAV->Free(m_srv);
}

void DXTexture2D::Init(ComPtr<ID3D12GraphicsCommandList> & commandList)
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

	// Create the GPU upload buffer.
	DX::Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(footPrintTotalBytes),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_uploadBuffer));

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

	D3D12_RANGE readRange = {};
	void * cpuPtr;
	m_uploadBuffer->Map(0, &readRange, &cpuPtr);
	memcpy(cpuPtr, &texture[0], 256*256*4);

	CD3DX12_TEXTURE_COPY_LOCATION dst(m_resource.Get(), 0);
	CD3DX12_TEXTURE_COPY_LOCATION src(m_uploadBuffer.Get(), footPrintLayout);
	commandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
    commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, 
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));
}

