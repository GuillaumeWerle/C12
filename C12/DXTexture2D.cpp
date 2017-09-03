#include "stdafx.h"
#include "DXTexture2D.h"

extern ID3D12Device * g_device;
	
DXTexture2D::DXTexture2D()
{
}

DXTexture2D::~DXTexture2D()
{
}

void DXTexture2D::Init()
{
	D3D12_RESOURCE_DESC rdesc = {};
	rdesc.MipLevels = 1;
	rdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rdesc.Width = 16;
	rdesc.Height = 16;
	rdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	rdesc.DepthOrArraySize = 1;
	rdesc.SampleDesc.Count = 1;
	rdesc.SampleDesc.Quality = 0;
	rdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	g_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&rdesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_resource));

	UINT64 footPrint = 0;
	g_device->GetCopyableFootprints(&rdesc, 0, 1, 0, nullptr, nullptr, nullptr, &footPrint);

	// Create the GPU upload buffer.
	g_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(footPrint),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_uploadBuffer));

	std::vector<UINT8> texture;
	texture.resize(footPrint);
	texture.assign(footPrint, 255);

	D3D12_RANGE readRange = {};
	void * cpuPtr;
	m_uploadBuffer->Map(0, &readRange, &cpuPtr);
	memcpy(cpuPtr, &texture[0], texture.size());




}

