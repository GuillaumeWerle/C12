#pragma once

class DXDescriptorHeap;
class DXDescriptorHeapLinear;
class DXBuffer;

class DXUploadContext
{
public:
	ID3D12Resource* Resource;
	u8* CPU;
	u64 GPU;
};

class DXResourceContext
{
public:

	std::array<DXDescriptorHeapLinear*, D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES> m_descriptorHeaps;
	DXBuffer * m_uploadBuffer = nullptr;
	DXUploadContext m_uploadStart;
	DXUploadContext m_uploadCurrent;

	DXDescriptorHeapLinear * GetCBVSRVUAVHeap() { return m_descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV]; }

	DXUploadContext AllocFromUploadHeap(u32 size);
	DXUploadContext AllocCB(u32 size) { return AllocFromUploadHeap(AlignOnPowerOfTwo<D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT>(size)); }

	void Init();
	void Reset();

	DXResourceContext();
	~DXResourceContext();
};

