#pragma once

class DXRenderer;
class DXDescriptorHeap;
class DXFence;
class DXRenderer;

class DXApp
{
public:
	static const u32 k_FrameCount = 2;
	uint32_t m_width = 1920;
	uint32_t m_height = 1080;

	void Init(HWND hWnd);
	void Update();
	void Render();

	DXApp();
	~DXApp();

private:

	void InitDebugLayer();

	std::array<DXDescriptorHeap*, D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES> m_descriptorHeaps;
	DXDescriptorHeap* m_swapChainBuffersDescriptorHeap = nullptr;

	u32 m_dxgiFactoryFlags;
	ComPtr<IDXGIFactory4> m_dxgiFactory;
	ComPtr<ID3D12Device> m_device;

	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12Resource> m_swapChainBuffers[k_FrameCount];

	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	// Synchronization objects.
	u32 m_frameIndex;
	std::unique_ptr<DXFence> m_fence;
	u64 m_fenceValue = 0;

	// Render
	DXRenderer* m_renderer;

};

