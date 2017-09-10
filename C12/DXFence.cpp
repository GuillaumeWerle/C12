#include "stdafx.h"
#include "DXFence.h"
#include "DX.h"

DXFence::DXFence()
{
}

DXFence::~DXFence()
{
	CloseHandle(m_fenceEvent);
}

void DXFence::Init(u64 initvalue)
{
	DX::Device->CreateFence(initvalue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	// Create an event handle to use for frame synchronization.
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	assert(m_fenceEvent);
}

void DXFence::WaitForGPU(ComPtr<ID3D12CommandQueue> & commandQueue, u64 value)
{
	commandQueue->Signal(m_fence.Get(), value);
	m_fence->SetEventOnCompletion(value, m_fenceEvent);
	WaitForSingleObject(m_fenceEvent, INFINITE);
}

