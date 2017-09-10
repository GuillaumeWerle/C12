#pragma once

#include "DXDescriptorHeap.h"

class DXDescriptorHeapLinear : public DXDescriptorHeap
{
public:

	u32 m_offset = 0;

	void Reset() { m_offset = 0;  }
	DXDescriptorHandle Alloc(u32 count = 1);

};

