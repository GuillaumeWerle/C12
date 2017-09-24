#include "stdafx.h"
#include "DX.h"
#include "DXApp.h"
#include "DXRenderer.h"
#include "DXDescriptorHeapLinear.h"
#include "DXDescriptorPool.h"
#include "DXRootSignature.h"
#include "DXFence.h"
#include "DXRenderer.h"
#include "DXBuffer.h"
#include "DXTexture2D.h"
#include "DXStructuredBuffer.h"
#include "Timer.h"
#include "DXRenderContext.h"
#include "MasterRenderThread.h"

DXApp* DXApp::ms_instance = nullptr;


DXApp::DXApp()
{
	assert(ms_instance == nullptr);
	ms_instance = this;
}

DXApp::~DXApp()
{
	m_masterRenderThread->EndThread();
	delete m_masterRenderThread;

	for (u32 i = 0; i < k_RenderLatency; i++)
	{
		m_renderContext[i]->Shutdown(m_commandQueue);
		delete m_renderContext[i];
	}

	delete DX::Uploader;

	delete m_timer;
	delete m_texture;
	delete m_renderer;
	delete m_swapChainBuffersDescriptorHeap;

	// descriptor pools must be released at the end.
	delete DX::PoolSRVCBVUAV;
	delete DX::PoolSampler;
	delete DX::PoolRTV;
	delete DX::PoolDSV;

	assert(ms_instance == this);
	ms_instance = nullptr;
}

void DXApp::ShutdownRendercontexts()
{
	for (u32 i = 0; i < k_RenderLatency; i++)
	{
		m_renderContext[i]->Shutdown(m_commandQueue);
	}
}

void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
{
	ComPtr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;

	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			// If you want a software adapter, pass in "/warp" on the command line.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*ppAdapter = adapter.Detach();
}


void DXApp::Init(HWND hWnd)
{
	m_timer = new Timer;

	m_dxgiFactoryFlags = 0;

#if defined(_DEBUG)
	InitDebugLayer();
#endif

	HRESULT hr = 0;
	CHECK_D3DOK(hr, CreateDXGIFactory2(m_dxgiFactoryFlags, IID_PPV_ARGS(&m_dxgiFactory)));

	ComPtr<IDXGIAdapter1> hardwareAdapter;
	GetHardwareAdapter(m_dxgiFactory.Get(), &hardwareAdapter);
	CHECK_D3DOK(hr, D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));
	DX::Device = m_device.Get();

	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	CHECK_D3DOK(hr, m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

	DX::PoolSRVCBVUAV = new DXDescriptorPool;
	DX::PoolSRVCBVUAV->Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 65535, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

	DX::PoolSampler = new DXDescriptorPool;
	DX::PoolSampler->Init(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 256, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

	DX::PoolDSV = new DXDescriptorPool;
	DX::PoolDSV->Init(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1024, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

	DX::PoolRTV = new DXDescriptorPool;
	DX::PoolRTV->Init(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 1024, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

	DX::Uploader = new DXUploader;

	InitSwapChain(hWnd);
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	m_renderer = new DXRenderer;
	m_renderer->Init();

	for (u32 i = 0; i < k_RenderLatency; i++)
	{
		m_renderContext[i] = new DXRenderContext;
		m_renderContext[i]->Init();
	}

	{
		m_texture = new DXTexture2D;
		m_texture->Init();
		DX::Uploader->RequestUpload(m_texture);
	}

	m_masterRenderThread = new MasterRenderThread(this);
	m_masterRenderThread->StartThread();

}

void DXApp::InitDebugLayer()
{
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			m_dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
}

void DXApp::InitSwapChain(HWND hWnd)
{
	HRESULT hr;
	m_swapChainBuffersDescriptorHeap = new DXDescriptorHeap;
	m_swapChainBuffersDescriptorHeap->Init(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, k_RenderLatency, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

	// Describe and create the swap chain.
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = k_RenderLatency;
	swapChainDesc.Width = m_width;
	swapChainDesc.Height = m_height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swapChain;
	CHECK_D3DOK(hr, m_dxgiFactory->CreateSwapChainForHwnd(
		m_commandQueue.Get(),		// Swap chain needs the queue so that it can force a flush on it.
		hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
	));
	swapChain.As(&m_swapChain);

	// This sample does not support fullscreen transitions.
	CHECK_D3DOK(hr, m_dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

	// Create a RTV for each frame.
	for (UINT bufferIndex = 0; bufferIndex < k_RenderLatency; bufferIndex++)
	{
		m_swapChain->GetBuffer(bufferIndex, IID_PPV_ARGS(&m_swapChainBuffers[bufferIndex]));
		m_swapChainRTVs[bufferIndex] = m_swapChainBuffersDescriptorHeap->GetHandle(bufferIndex);
		DX::Device->CreateRenderTargetView(m_swapChainBuffers[bufferIndex].Get(), nullptr, m_swapChainRTVs[bufferIndex].CPU);
	}
}

void DXApp::Update()
{
	m_timer->Tick();
}

void DXApp::Render()
{
	if (DX::Device == nullptr)
		return;

	DXRenderContext * rc = m_renderContext[m_frameIndex];
	rc->Reset(m_commandQueue);

	DX::Uploader->ExecuteUploadRequests(rc);

	rc->ResourceBarrier(m_swapChainBuffers[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	DXDescriptorHandle & swapChainRTV = m_swapChainRTVs[m_frameIndex];
	rc->ClearRTV(swapChainRTV, XMFLOAT4(0.2f, 0.3f, 0.7f, 1.0f));
	rc->SetRenderTarget(swapChainRTV);
	CD3DX12_VIEWPORT viewport(0.0f, 0.0f, (float)m_width, (float)m_height);
	CD3DX12_RECT scissorRect(0, 0, m_width, m_height);
	rc->SetViewport(viewport);
	rc->SetScissorRect(scissorRect);

	rc->SetGraphicRootSignature(m_renderer->m_rootSignature);
	rc->SetPipelineState(m_renderer->m_pso.Get());
	struct cblocal
	{
		XMFLOAT4 color;
		XMFLOAT4 offset;
	};
	cblocal cb;
	cb.color = XMFLOAT4(0.5f, 0.6f, 0.7f, 1.0f);
	cb.offset = XMFLOAT4(0.25f * sinf((float)m_timer->GetTimeSinceStart()), 0, 0, 0);

	rc->SetCB(ERootParamIndex::CBGlobal, &cb, sizeof(cb));
	rc->SetDescriptorTable(ERootParamIndex::SRVTable, &m_texture->m_srv, 1);

	DXDescriptorHandle vertexStreams[] = 
	{ 
		m_renderer->m_streamPos->m_srv,
		m_renderer->m_streamUV->m_srv,
	};
	rc->SetDescriptorTable(ERootParamIndex::SRVVertexStreamsTable, vertexStreams, _countof(vertexStreams));


	rc->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	rc->SetVertexBuffers(0, 1, &m_renderer->m_vertexBufferView);
	rc->DrawInstanced(3, 1, 0, 0);

	// Indicate that the back buffer will now be used to present.
	rc->ResourceBarrier(m_swapChainBuffers[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	rc->Close();
	rc->Execute(m_commandQueue);

	// Present the frame.
	m_swapChain->Present(1, 0);
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

}