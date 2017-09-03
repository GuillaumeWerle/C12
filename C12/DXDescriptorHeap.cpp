#include "stdafx.h"
#include "DXDescriptorHeap.h"


extern ID3D12Device * g_device;


DXDescriptorHeap::DXDescriptorHeap()
{
}


DXDescriptorHeap::~DXDescriptorHeap()
{
}

DXDescriptorHandle DXDescriptorHeap::Alloc(u32 count)
{
	DXDescriptorHandle h;
	h.CPU = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cpuPtr, m_offset, m_incrementSize);
	h.GPU = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_gpuPtr, m_offset, m_incrementSize);
	m_offset += count;
	return h;
}

void DXDescriptorHeap::Init(D3D12_DESCRIPTOR_HEAP_TYPE type, u32 count, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
{
	HRESULT hr = 0;
	memset(&m_desc, 0, sizeof(m_desc));
	m_desc.Type = type;
	m_desc.NumDescriptors = count;
	m_desc.Flags = flags;
	CHECK_D3DOK(hr, g_device->CreateDescriptorHeap(&m_desc, IID_PPV_ARGS(&m_heap)));

	m_cpuPtr = m_heap->GetCPUDescriptorHandleForHeapStart();
	m_gpuPtr = m_heap->GetGPUDescriptorHandleForHeapStart();
	m_incrementSize = g_device->GetDescriptorHandleIncrementSize(type);
}

void DXDescriptorHeap::Reset()
{
	m_offset = 0;
}

