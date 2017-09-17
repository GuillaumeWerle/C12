#pragma once

class DXFence
{
public:

	u64 GetCompletedValue() 
	{
		return m_fence->GetCompletedValue();
	}

	void Init(u64 initvalue);

	void Signal(ComPtr<ID3D12CommandQueue> & commandQueue, u64 value);
	void Wait(ComPtr<ID3D12CommandQueue> & commandQueue, u64 value);
	void Sync(ComPtr<ID3D12CommandQueue> & commandQueue, u64 value);

	DXFence();
	~DXFence();

private:
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
};

