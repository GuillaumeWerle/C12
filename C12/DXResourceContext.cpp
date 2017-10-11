#include "stdafx.h"
#include "DXResourceContext.h"
#include "DXDescriptorHeapLinear.h"
#include "DXBufferHeap.h"

DXResourceContext::DXResourceContext()
{
	m_descriptorHeaps.assign(nullptr);
}

DXResourceContext::~DXResourceContext()
{
	delete m_uploadBuffer;

	for (auto & heap : m_descriptorHeaps)
		delete heap;
}

void DXResourceContext::Init()
{
	u32 heapSizes[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] = {};
	heapSizes[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV] = 16384;
	heapSizes[D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER] = 256;
	heapSizes[D3D12_DESCRIPTOR_HEAP_TYPE_RTV] = 4096;
	heapSizes[D3D12_DESCRIPTOR_HEAP_TYPE_DSV] = 4096;

	for (u32 i = 0; i < _countof(heapSizes); i++)
	{
		assert(heapSizes[i]);

		D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		if (i == D3D12_DESCRIPTOR_HEAP_TYPE_RTV || i == D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
			flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		m_descriptorHeaps[i] = new DXDescriptorHeapLinear;
		m_descriptorHeaps[i]->Init((D3D12_DESCRIPTOR_HEAP_TYPE)i, heapSizes[i], flags);
	}

	m_uploadBuffer = new DXBufferHeap;
	m_uploadBuffer->Init(D3D12_HEAP_TYPE_UPLOAD, 32 * 1024 * 1024);
}

void DXResourceContext::Reset()
{
	for (auto & heap : m_descriptorHeaps)
		heap->Reset();

	m_uploadCurrent.Resource = m_uploadBuffer->GetResource();
	m_uploadCurrent.CPU = m_uploadBuffer->Map();
	m_uploadCurrent.GPU = m_uploadBuffer->GetGPUVirtualAddress();
}

DXUploadContext DXResourceContext::AllocFromUploadHeap(u32 size)
{
	const u32 alignedSize = AlignOnPowerOfTwo<256>(size);
	DXUploadContext ctx = m_uploadCurrent;
	m_uploadCurrent.CPU += alignedSize;
	m_uploadCurrent.GPU += alignedSize;
	return ctx;
}
