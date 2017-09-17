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

void DXFence::Signal(ComPtr<ID3D12CommandQueue> & commandQueue, u64 value)
{	
	commandQueue->Signal(m_fence.Get(), value);
}

void DXFence::Wait(ComPtr<ID3D12CommandQueue> & commandQueue, u64 value)
{
	if (m_fence->GetCompletedValue() == value)
		return;

	m_fence->SetEventOnCompletion(value, m_fenceEvent);
	WaitForSingleObject(m_fenceEvent, INFINITE);
}

void DXFence::Sync(ComPtr<ID3D12CommandQueue> & commandQueue, u64 value)
{
	Signal(commandQueue, value);
	Wait(commandQueue, value);
}

