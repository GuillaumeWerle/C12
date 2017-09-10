#include "stdafx.h"
#include "DXResourceContext.h"
#include "DXDescriptorHeapLinear.h"

DXResourceContext::DXResourceContext()
{
	m_descriptorHeaps.assign(nullptr);

}

DXResourceContext::~DXResourceContext()
{
	for (size_t i = 0; i < m_descriptorHeaps.size(); i++)
	{
		delete m_descriptorHeaps[i];
	}
}

void DXResourceContext::Reset()
{
	for (auto & heap : m_descriptorHeaps)
		heap->Reset();
}
