#pragma once

#include "DXDescriptorHandle.h"
#include "DXRootSignature.h"

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

	ID3D12GraphicsCommandList * GetCommandList() { return m_commandList.Get(); }

	void Init();
	void Reset(ComPtr<ID3D12CommandQueue> & queue);
	void Close();
	void Execute(ComPtr<ID3D12CommandQueue> & queue);
	void Shutdown(ComPtr<ID3D12CommandQueue> & queue);

	void ClearRTV(DXDescriptorHandle rtv, XMFLOAT4 color);
	void SetRenderTarget(DXDescriptorHandle rtv);
	void SetViewport(const CD3DX12_VIEWPORT & viewport);
	void SetScissorRect(const CD3DX12_RECT & rect);
	void SetScissorRects(const CD3DX12_RECT * rects, u32 count);

	void SetGraphicRootSignature(DXRootSignature * rootSignature);
	void SetCB(ERootParamIndex index, void * ptr, u32 size);
	void SetSRVTable(DXDescriptorHandle * srvs, u32 count);
	void SetPipelineState(ID3D12PipelineState * pso);
	void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology);
	void SetVertexBuffers(u32 start, u32 count, D3D12_VERTEX_BUFFER_VIEW * vb);
	void DrawInstanced(UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
	void ResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);
	void ResourceBarriers(UINT NumBarriers, D3D12_RESOURCE_BARRIER *pBarriers);

	DXRenderContext();
	~DXRenderContext();
};

