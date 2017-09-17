#pragma once

class DXFence;
class DXResourceContext;

class DXRenderContext
{
public:
	DXFence * m_fence = nullptr;
	u64 m_fenceValue = 0;
	DXResourceContext * m_resource = nullptr;

	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<ID3D12PipelineState> m_psoNull;

	void Init();
	void Begin(ComPtr<ID3D12CommandQueue> & queue);
	void End(ComPtr<ID3D12CommandQueue> & queue);

	DXRenderContext();
	~DXRenderContext();
};

