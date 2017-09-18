#include "stdafx.h"
#include "DXRenderContext.h"
#include "DXFence.h"
#include "DX.h"
#include "DXResourceContext.h"
#include "DXRootSignature.h"

DXRenderContext::DXRenderContext()
{
}

DXRenderContext::~DXRenderContext()
{
	delete m_fence;
	delete m_resource;
}

void DXRenderContext::Shutdown(ComPtr<ID3D12CommandQueue> & queue)
{
	m_fence->Wait(queue, m_fenceValue);
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

void DXRenderContext::Reset(ComPtr<ID3D12CommandQueue> & queue)
{
	m_fence->Wait(queue, m_fenceValue);

	m_resource->Reset();
	m_commandAllocator->Reset();
	m_commandList->Reset(m_commandAllocator.Get(), m_psoNull.Get());

	ID3D12DescriptorHeap* ppHeaps[] = { m_resource->GetCBVSRVUAVHeap()->m_heap.Get() };
	m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void DXRenderContext::Close()
{
	m_commandList->Close();
}

void DXRenderContext::Execute(ComPtr<ID3D12CommandQueue> & queue)
{
	ID3D12CommandList* ppCommandLists[] = { GetCommandList() };
	queue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	m_fence->Signal(queue, ++m_fenceValue);
}

void DXRenderContext::ClearRTV(DXDescriptorHandle rtv, XMFLOAT4 color)
{
	m_commandList->ClearRenderTargetView(rtv.CPU, (float*)&color, 0, nullptr);
}

void DXRenderContext::SetRenderTarget(DXDescriptorHandle rtv)
{
	m_commandList->OMSetRenderTargets(1, &rtv.CPU, FALSE, nullptr);
}

void DXRenderContext::SetGraphicRootSignature(DXRootSignature * rootSignature)
{
	m_commandList->SetGraphicsRootSignature(rootSignature->Get());
}

void DXRenderContext::SetCB(ERootParamIndex index, void* ptr, u32 size)
{
	DXUploadContext m = m_resource->AllocCB(size);
	memcpy(m.CPU, ptr, size);
	m_commandList->SetGraphicsRootConstantBufferView((UINT)index, m.GPU);
}

void DXRenderContext::SetSRVTable(DXDescriptorHandle* srvs, u32 count)
{
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandles[16] = {};
	assert(count < _countof(srvHandles));
	for (u32 i = 0; i < count; i++)
		srvHandles[i] = srvs[i].CPU;

	DXDescriptorHandle tableSRV = m_resource->GetCBVSRVUAVHeap()->Alloc(count);
	static u32 destRanges[1] = { count };
	static const u32 DescriptorCopyRanges[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	DX::Device->CopyDescriptors(1, &tableSRV.CPU, destRanges, count, srvHandles, DescriptorCopyRanges, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_commandList->SetGraphicsRootDescriptorTable(ERootParamIndex::SRVTable, tableSRV.GPU);
}

void DXRenderContext::SetPipelineState(ID3D12PipelineState * pso)
{
	m_commandList->SetPipelineState(pso);
}

void DXRenderContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
{
	m_commandList->IASetPrimitiveTopology(topology);
}

void DXRenderContext::SetVertexBuffers(u32 start, u32 count, D3D12_VERTEX_BUFFER_VIEW * vb)
{
	m_commandList->IASetVertexBuffers(start, count, vb);
}

void DXRenderContext::DrawInstanced(UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation)
{
	m_commandList->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

void DXRenderContext::ResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
{
	ResourceBarriers(1, &CD3DX12_RESOURCE_BARRIER::Transition(pResource, stateBefore, stateAfter));
}

void DXRenderContext::ResourceBarriers(UINT NumBarriers, D3D12_RESOURCE_BARRIER *pBarriers)
{
	m_commandList->ResourceBarrier(NumBarriers, pBarriers);
}

void DXRenderContext::SetViewport(const CD3DX12_VIEWPORT & viewport)
{
	m_commandList->RSSetViewports(1, &viewport);
}

void DXRenderContext::SetScissorRect(const CD3DX12_RECT & rect)
{
	SetScissorRects(&rect, 1);
}

void DXRenderContext::SetScissorRects(const CD3DX12_RECT * rects, u32 count)
{
	m_commandList->RSSetScissorRects(count, rects);
}

