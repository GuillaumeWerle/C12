#include "stdafx.h"
#include "DXRenderContext.h"
#include "DXFence.h"
#include "DX.h"
#include "DXResourceContext.h"
#include "DXRootSignature.h"

DXRenderContext::DXRenderContext()
{
	//m_tmpVertexStreamSrvHandles.resize((size_t)EVertexSteam::Count);
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
	u32 srvTableSize = Max<u32>(rootSignature->GetSRVCount(), (u32)EVertexSteam::Count);
	m_tmpSrvHandles.resize(srvTableSize);
	D3D12_CPU_DESCRIPTOR_HANDLE emptyHandle;
	emptyHandle.ptr = 0;
	for (auto & h : m_tmpSrvHandles)
		h = emptyHandle;
}

void DXRenderContext::SetCB(ERootParamIndex index, void* ptr, u32 size)
{
	DXUploadContext m = m_resource->AllocCB(size);
	memcpy(m.CPU, ptr, size);
	m_commandList->SetGraphicsRootConstantBufferView((UINT)index, m.GPU);
}

void DXRenderContext::SetDescriptorTable(ERootParamIndex index, const DXDescriptorHandle * srvs, u32 count)
{
	assert(count);
	assert(count < m_tmpSrvHandles.size());

	// copy the SRVs to the temporary storage
	for (u32 i = 0; i < count; i++)
		m_tmpSrvHandles[i] = srvs[i].CPU;

	// fill the rest of the temporary storage if needed
	for (u32 i = count; i < m_tmpSrvHandles.size(); i++)
		m_tmpSrvHandles[i] = srvs[0].CPU;

	DXDescriptorHandle tableSRV = m_resource->GetCBVSRVUAVHeap()->Alloc((u32)m_tmpSrvHandles.size());
	u32 destRanges[] = { (u32)m_tmpSrvHandles.size() };
	static const u32 descriptorCopyRanges[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	assert(_countof(descriptorCopyRanges) > m_tmpSrvHandles.size());
	DX::Device->CopyDescriptors(1, &tableSRV.CPU, destRanges, (u32)m_tmpSrvHandles.size(), &m_tmpSrvHandles[0], descriptorCopyRanges, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_commandList->SetGraphicsRootDescriptorTable((u32)index, tableSRV.GPU);
}

//void DXRenderContext::SetVertexStreamsSRVTable(DXDescriptorHandle * srvs, u32 count)
//{
//	assert(count < m_tmpVertexStreamSrvHandles.size());
//
//	// copy the SRVs to the temporary storage
//	for (u32 i = 0; i < count; i++)
//		m_tmpVertexStreamSrvHandles[i] = srvs[i].CPU;
//
//	// fill the rest of the temporary storage if needed
//	for (u32 i = count; i < m_tmpVertexStreamSrvHandles.size(); i++)
//		m_tmpVertexStreamSrvHandles[i] = srvs[0].CPU;
//
//	DXDescriptorHandle tableSRV = m_resource->GetCBVSRVUAVHeap()->Alloc((u32)m_tmpVertexStreamSrvHandles.size());
//	u32 destRanges[] = { (u32)m_tmpVertexStreamSrvHandles.size() };
//	static const u32 descriptorCopyRanges[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
//	assert(_countof(descriptorCopyRanges) > m_tmpVertexStreamSrvHandles.size());
//	DX::Device->CopyDescriptors(1, &tableSRV.CPU, destRanges, (u32)m_tmpVertexStreamSrvHandles.size(), &m_tmpVertexStreamSrvHandles[0], descriptorCopyRanges, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//	m_commandList->SetGraphicsRootDescriptorTable((u32)ERootParamIndex::VertexStreamsSRVTable, tableSRV.GPU);
//}

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

void DXRenderContext::CopyTextureRegion(const D3D12_TEXTURE_COPY_LOCATION *pDst, UINT DstX, UINT DstY, UINT DstZ, const D3D12_TEXTURE_COPY_LOCATION *pSrc, const D3D12_BOX *pSrcBox)
{
	m_commandList->CopyTextureRegion(pDst, DstX, DstY, DstZ, pSrc, pSrcBox);
}

void DXRenderContext::CopyBufferRegion(ID3D12Resource *pDstBuffer, UINT64 DstOffset, ID3D12Resource *pSrcBuffer, UINT64 SrcOffset, UINT64 NumBytes)
{
	m_commandList->CopyBufferRegion(pDstBuffer, DstOffset, pSrcBuffer, SrcOffset, NumBytes);
}

DXUploadContext DXRenderContext::AllocFromUploadHeap(u32 size)
{
	return m_resource->AllocFromUploadHeap(size);
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

