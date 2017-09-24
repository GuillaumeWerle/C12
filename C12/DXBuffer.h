#pragma once
class DXBuffer
{
public:
	D3D12_RESOURCE_DESC m_desc = {};
	ComPtr<ID3D12Resource> m_buffer;
	u8 * m_cpuPtr = nullptr; 
	D3D12_GPU_VIRTUAL_ADDRESS m_gpuPtr = 0;

	void Init(D3D12_HEAP_TYPE heapType, u64 sizeInByte);

	DXBuffer();
	~DXBuffer();
};

