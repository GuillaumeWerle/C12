#pragma once
#include "DXResourceContext.h"

class DXRenderer;
class DXDescriptorHeap;
class DXFence;
class DXRenderer;
class DXTexture2D;

class DXApp
{
public:
	static const u32 k_RenderLatency = 2;
	uint32_t m_width = 1920;
	uint32_t m_height = 1080;

	void Init(HWND hWnd);
	void Update();
	void Render();

	DXApp();
	~DXApp();

private:

	void InitDebugLayer();

	DXDescriptorHeap* m_swapChainBuffersDescriptorHeap = nullptr;
	DXResourceContext m_resourceContexts[k_RenderLatency];
	DXResourceContext * m_rc;

	u32 m_dxgiFactoryFlags;
	ComPtr<IDXGIFactory4> m_dxgiFactory;
	ComPtr<ID3D12Device> m_device;

	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12Resource> m_swapChainBuffers[k_RenderLatency];

	ComPtr<ID3D12PipelineState> m_psoNull;
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	// Synchronization objects.
	u32 m_frameIndex;
	std::unique_ptr<DXFence> m_fence;
	u64 m_fenceValue = 0;

	// Render
	DXRenderer* m_renderer;
	DXTexture2D * m_texture = nullptr;

};

