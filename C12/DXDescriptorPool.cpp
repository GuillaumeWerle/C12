#include "stdafx.h"
#include "DXDescriptorPool.h"

void DXDescriptorPool::Init(D3D12_DESCRIPTOR_HEAP_TYPE type, u32 count, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
{
	m_linearHeap.Init(type, count, flags);

	m_handles.reserve(count);
	for (u32 i = 0; i < count; i++)
		m_handles.push_back(m_linearHeap.Alloc(1));
}

DXDescriptorHandle DXDescriptorPool::Alloc()
{
	assert(m_handles.empty() == false);
	DXDescriptorHandle h = m_handles.back();
	m_handles.pop_back();
	return h;
}

void DXDescriptorPool::Free(DXDescriptorHandle & handle)
{
	m_handles.push_back(handle);
}

