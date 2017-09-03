#pragma once
class DXBuffer
{
public:
	D3D12_RESOURCE_DESC m_desc;
	ComPtr<ID3D12Resource> m_buffer;
	u8 * m_cpuPtr;
	D3D12_GPU_VIRTUAL_ADDRESS m_gpuPtr;

	void Init(D3D12_HEAP_TYPE heapType, u32 sizeInByte);

	DXBuffer();
	~DXBuffer();
};

