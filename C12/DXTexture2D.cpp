#include "stdafx.h"
#include "DXTexture2D.h"

extern ID3D12Device * g_device;
	
DXTexture2D::DXTexture2D()
{
}

DXTexture2D::~DXTexture2D()
{
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

	g_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&rdesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_resource));

	UINT64 footPrintTotalBytes = 0;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footPrintLayout;
	UINT numRows;
	UINT64 rowSizeInBytes;
	g_device->GetCopyableFootprints(&rdesc, 0, 1, 0, &footPrintLayout, &numRows, &rowSizeInBytes, &footPrintTotalBytes);

	// Create the GPU upload buffer.
	g_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(footPrintTotalBytes),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_uploadBuffer));

	std::vector<UINT8> texture;
	texture.resize(footPrintTotalBytes);
	texture.assign(footPrintTotalBytes, 255);

	D3D12_RANGE readRange = {};
	void * cpuPtr;
	m_uploadBuffer->Map(0, &readRange, &cpuPtr);
	memcpy(cpuPtr, &texture[0], texture.size());

	D3D12_TEXTURE_COPY_LOCATION dst;
	dst.pResource = m_resource.Get();
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = 0;

	D3D12_TEXTURE_COPY_LOCATION src;
	src.pResource = m_uploadBuffer.Get();
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint = footPrintLayout;

	commandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
    commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}
