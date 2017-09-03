#pragma once

class DXDescriptorHeap;

class DXResourceContext
{
public:

	std::array<DXDescriptorHeap*, D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES> m_descriptorHeaps;

	DXDescriptorHeap * GetCBVSRVUAVHeap() { return m_descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV]; }
	void Reset();

	DXResourceContext();
	~DXResourceContext();
};

