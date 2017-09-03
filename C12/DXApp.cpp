#include "stdafx.h"
#include "DXApp.h"
#include "DXRenderer.h"
#include "DXDescriptorHeap.h"
#include "DXFence.h"
#include "DXRenderer.h"
#include "DXBuffer.h"

ID3D12Device * g_device = nullptr;

DXApp::DXApp()
{
}

DXApp::~DXApp()
{
	delete m_renderer;

	delete m_swapChainBuffersDescriptorHeap;
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
	m_dxgiFactoryFlags = 0;

#if defined(_DEBUG)
	InitDebugLayer();
#endif

	HRESULT hr = 0;
	CHECK_D3DOK(hr, CreateDXGIFactory2(m_dxgiFactoryFlags, IID_PPV_ARGS(&m_dxgiFactory)));

	ComPtr<IDXGIAdapter1> hardwareAdapter;
	GetHardwareAdapter(m_dxgiFactory.Get(), &hardwareAdapter);
	CHECK_D3DOK(hr, D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));
	g_device = m_device.Get();

	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	CHECK_D3DOK(hr, m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

	u32 heapSizes[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] = {};
	heapSizes[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV] = 16384;
	heapSizes[D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER] = 256;
	heapSizes[D3D12_DESCRIPTOR_HEAP_TYPE_RTV] = 4096;
	heapSizes[D3D12_DESCRIPTOR_HEAP_TYPE_DSV] = 4096;

	for (auto & res : m_resourceContexts)
	{
		for (u32 i = 0; i < _countof(heapSizes); i++)
		{
			assert(heapSizes[i]);
			res.m_descriptorHeaps[i] = new DXDescriptorHeap;

			D3D12_DESCRIPTOR_HEAP_FLAGS flags;
			if (i == D3D12_DESCRIPTOR_HEAP_TYPE_RTV || i == D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
				flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			else
				flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

			res.m_descriptorHeaps[i]->Init((D3D12_DESCRIPTOR_HEAP_TYPE)i, heapSizes[i], flags);
		}
	}

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

	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	// Create a RTV for each frame.
	for (UINT n = 0; n < k_RenderLatency; n++)
	{
		m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_swapChainBuffers[n]));
		m_device->CreateRenderTargetView(m_swapChainBuffers[n].Get(), nullptr, m_swapChainBuffersDescriptorHeap->GetCpuHandle(n));
	}

	m_fence = std::make_unique<DXFence>();
	m_fence->Init(m_fenceValue);

	m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
	m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList));
	m_commandList->Close();

	m_renderer = new DXRenderer;
	m_renderer->Init();


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

void DXApp::Update()
{

}

void DXApp::Render()
{
	if (g_device == nullptr)
		return;

	DXResourceContext & rc = m_resourceContexts[m_frameIndex];
	m_rc = &rc;
	rc.Reset();

	m_commandAllocator->Reset();
	m_commandList->Reset(m_commandAllocator.Get(), m_psoNull.Get());

	// Indicate that the back buffer will be used as a render target.
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_swapChainBuffers[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_swapChainBuffersDescriptorHeap->GetCpuHandle(m_frameIndex);

	// Record commands.
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
	
	m_commandList->SetGraphicsRootSignature(m_renderer->m_rootSignature.Get());

	m_commandList->SetGraphicsRootConstantBufferView(1, m_renderer->m_cb->m_gpuPtr);
	//m_commandList->SetGraphicsRootShaderResourceView(2, )

	CD3DX12_VIEWPORT viewport(0.0f, 0.0f, (float)m_width, (float)m_height);
	CD3DX12_RECT scissorRect(0, 0, m_width, m_height);

	m_commandList->RSSetViewports(1, &viewport);
	m_commandList->RSSetScissorRects(1, &scissorRect);

	m_commandList->SetPipelineState(m_renderer->m_pso.Get());
	m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_commandList->IASetVertexBuffers(0, 1, &m_renderer->m_vertexBufferView);
	m_commandList->DrawInstanced(3, 1, 0, 0);


	// Indicate that the back buffer will now be used to present.
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_swapChainBuffers[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	m_commandList->Close();

	// Execute the command list.
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Present the frame.
	m_swapChain->Present(1, 0);

	m_fenceValue++;
	m_fence->WaitForGPU(m_commandQueue, m_fenceValue);
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}
