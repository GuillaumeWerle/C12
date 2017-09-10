#include "stdafx.h"
#include "DXDescriptorHeap.h"
#include "DX.h"



DXDescriptorHeap::DXDescriptorHeap()
{
}


DXDescriptorHeap::~DXDescriptorHeap()
{
}


void DXDescriptorHeap::Init(D3D12_DESCRIPTOR_HEAP_TYPE type, u32 count, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
{
	HRESULT hr = 0;
	memset(&m_desc, 0, sizeof(m_desc));
	m_desc.Type = type;
	m_desc.NumDescriptors = count;
	m_desc.Flags = flags;
	CHECK_D3DOK(hr, DX::Device->CreateDescriptorHeap(&m_desc, IID_PPV_ARGS(&m_heap)));

	m_cpuPtr = m_heap->GetCPUDescriptorHandleForHeapStart();
	m_gpuPtr = m_heap->GetGPUDescriptorHandleForHeapStart();
	m_incrementSize = DX::Device->GetDescriptorHandleIncrementSize(type);
}
