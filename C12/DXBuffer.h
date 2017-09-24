#pragma once
class DXBuffer
{
public:
	const D3D12_RESOURCE_DESC & GetDesc() const { return m_desc; }
	ID3D12Resource * GetResource() { return m_buffer.Get(); }
	u8 * Map() { assert(m_cpuPtr); return m_cpuPtr; }
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() { return m_gpuPtr; }

	void Init(D3D12_HEAP_TYPE heapType, u64 sizeInByte);

	DXBuffer();
	~DXBuffer();

private:
	D3D12_RESOURCE_DESC m_desc = {};
	ComPtr<ID3D12Resource> m_buffer;
	u8 * m_cpuPtr = nullptr;
	D3D12_GPU_VIRTUAL_ADDRESS m_gpuPtr = 0;
};

