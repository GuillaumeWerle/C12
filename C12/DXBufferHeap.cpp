#include "stdafx.h"
#include "DXBufferHeap.h"
#include "DXHelpers.h"
#include "DX.h"

DXBufferHeap::DXBufferHeap()
{
}


DXBufferHeap::~DXBufferHeap()
{
}

void DXBufferHeap::Init(D3D12_HEAP_TYPE heapType, u64 sizeInByte)
{
	D3D12_RESOURCE_DESC desc = {};
	desc.Alignment = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Height = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;	// linear pitch
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Width = sizeInByte;
	m_desc = desc;

	D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_GENERIC_READ;

	const D3D12_HEAP_PROPERTIES * heapProperties = nullptr;
	switch (heapType)
	{
	case D3D12_HEAP_TYPE_DEFAULT:
		initialState = D3D12_RESOURCE_STATE_COPY_DEST;
		heapProperties = DXHelpers::GetDefaultHeapProps();
		break;
	case D3D12_HEAP_TYPE_UPLOAD:
		heapProperties = DXHelpers::GetUploadHeapProps();
		break;
	case D3D12_HEAP_TYPE_READBACK:
		heapProperties = DXHelpers::GetReadbackHeapProps();
		break;
	default:
	case D3D12_HEAP_TYPE_CUSTOM:
		assert(0);	// not handled
		break;
	}

	HRESULT hr;
	CHECK_D3DOK(hr, DX::Device->CreateCommittedResource(heapProperties, D3D12_HEAP_FLAG_NONE, &desc, initialState, nullptr, IID_PPV_ARGS(&m_buffer)));
	m_gpuPtr = m_buffer->GetGPUVirtualAddress();

	if (heapType == D3D12_HEAP_TYPE_UPLOAD)
	{
		D3D12_RANGE readRange = {};
		m_buffer->Map(0, &readRange, (void**)&m_cpuPtr);
	}
}
