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
	std::lock_guard<std::mutex> lock(m_mutex);
	assert(m_handles.empty() == false);	// out of stock ?
	DXDescriptorHandle h = m_handles.back();
	m_handles.pop_back();
	return h;
}

void DXDescriptorPool::Free(DXDescriptorHandle & handle)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_handles.push_back(handle);
}

