#pragma once

#include "DXDescriptorHeap.h"

class DXDescriptorHeapLinear : public DXDescriptorHeap
{
public:

	DXDescriptorHandle Alloc(u32 count = 1);
	void Reset() { m_offset = 0;  }

private:
	u32 m_offset = 0;
};

