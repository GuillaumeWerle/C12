#pragma once

#include "DXDescriptorHeapLinear.h"

class DXDescriptorPool 
{
public:
	DXDescriptorHandle Alloc();
	void Free(DXDescriptorHandle & handle);

	void Init(D3D12_DESCRIPTOR_HEAP_TYPE type, u32 count, D3D12_DESCRIPTOR_HEAP_FLAGS flags);

private:
	std::mutex m_mutex;
	DXDescriptorHeapLinear m_linearHeap;
	std::vector<DXDescriptorHandle> m_handles;
};

