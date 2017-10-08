#pragma once
#include "DXResourceContext.h"
#include "DXDescriptorHandle.h"
#include "DXSRV.h"

class DXRenderer;
class DXDescriptorHeap;
class DXDescriptorHeapLinear;
class DXDescriptorPool;
class DXFence;
class DXRenderer;
class DXTexture2D;
class Timer;
class DXRenderContext;
class MasterRenderThread;
class DXDepthStencil;

class DXApp
{
public:
	static DXApp* ms_instance;

	static const u32 k_RenderLatency = 2;
	uint32_t m_width = 1920;
	uint32_t m_height = 1080;

	void Init(HWND hWnd);

	void Update();
	void Render();

	DXApp();
	~DXApp();

	void ShutdownRendercontexts();

private:

	//DXDescriptorHeap* m_swapChainBuffersDescriptorHeap = nullptr;
	DXRTV m_swapChainRTVs[k_RenderLatency];
	//DXResourceContext m_resourceContexts[k_RenderLatency];
	DXResourceContext* m_rc;
	DXRenderContext* m_renderContexts[k_RenderLatency];

	u32 m_dxgiFactoryFlags;
	ComPtr<IDXGIFactory4> m_dxgiFactory;
	ComPtr<ID3D12Device> m_device;

	DXDepthStencil* m_depthStencil = nullptr;
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12Resource> m_swapChainBuffers[k_RenderLatency];
	ComPtr<ID3D12CommandQueue> m_commandQueue;

	u32 m_frameIndex;

	// Render
	DXRenderer* m_renderer;
	DXTexture2D* m_texture = nullptr;

	Timer * m_timer = nullptr;
	MasterRenderThread * m_masterRenderThread = nullptr;

	void InitDebugLayer();
	void InitSwapChain(HWND hWnd);
};

