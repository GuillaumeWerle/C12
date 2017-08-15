#pragma once


class DXDescriptorHeap
{
public:

	ComPtr<ID3D12DescriptorHeap> m_heap;
	D3D12_DESCRIPTOR_HEAP_DESC m_desc;
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpuPtr;
	D3D12_GPU_DESCRIPTOR_HANDLE m_gpuPtr;
	u32 m_incrementSize;

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(u32 index)
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cpuPtr, index, m_incrementSize);
	}

	CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(u32 index)
	{
		return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_gpuPtr, index, m_incrementSize);
	}

	void Init(D3D12_DESCRIPTOR_HEAP_TYPE type, u32 count, D3D12_DESCRIPTOR_HEAP_FLAGS flags);

	DXDescriptorHeap();
	~DXDescriptorHeap();
};

