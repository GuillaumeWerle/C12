#pragma once

class DXDescriptorHeap;
class DXDescriptorHeapLinear;

class DXResourceContext
{
public:

	std::array<DXDescriptorHeapLinear*, D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES> m_descriptorHeaps;

	DXDescriptorHeapLinear * GetCBVSRVUAVHeap() { return m_descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV]; }

	void Reset();

	DXResourceContext();
	~DXResourceContext();
};

