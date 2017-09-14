#include "stdafx.h"
#include "DXDescriptorHeapLinear.h"


DXDescriptorHandle DXDescriptorHeapLinear::Alloc(u32 count)
{
	DXDescriptorHandle h;
	h.CPU = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cpu, m_offset, m_incrementSize);
	h.GPU = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_gpu, m_offset, m_incrementSize);
	m_offset += count;
	return h;
}
