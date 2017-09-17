#include "stdafx.h"
#include "DXRenderContext.h"
#include "DXFence.h"
#include "DX.h"
#include "DXResourceContext.h"

DXRenderContext::DXRenderContext()
{
}

DXRenderContext::~DXRenderContext()
{
	delete m_fence;
	delete m_resource;
}

void DXRenderContext::Init()
{
	m_resource = new DXResourceContext;
	m_resource->Init();

	m_fence = new DXFence;
	m_fence->Init(m_fenceValue);

	DX::Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
	DX::Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList));
	m_commandList->Close();
}

void DXRenderContext::Begin(ComPtr<ID3D12CommandQueue> & queue)
{
	m_fence->Wait(queue, m_fenceValue);
	m_fence->Signal(queue, ++m_fenceValue);
	m_resource->Reset();
	m_commandAllocator->Reset();
	m_commandList->Reset(m_commandAllocator.Get(), m_psoNull.Get());

	ID3D12DescriptorHeap* ppHeaps[] = { m_resource->GetCBVSRVUAVHeap()->m_heap.Get() };
	m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}


void DXRenderContext::End(ComPtr<ID3D12CommandQueue> & queue)
{
	m_commandList->Close();

	ID3D12CommandList * lists[] = { m_commandList.Get() };
	queue->ExecuteCommandLists(1, lists);
}

