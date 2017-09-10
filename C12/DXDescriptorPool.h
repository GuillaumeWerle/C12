#pragma once

#include "DXDescriptorHeapLinear.h"

class DXDescriptorPool 
{
public:

	DXDescriptorHeapLinear m_linearHeap;
	std::vector<DXDescriptorHandle> m_handles;

	DXDescriptorHandle Alloc();
	void Free(DXDescriptorHandle & handle);

	void Init(D3D12_DESCRIPTOR_HEAP_TYPE type, u32 count, D3D12_DESCRIPTOR_HEAP_FLAGS flags);


};

