#include "stdafx.h"
#include "DXStructuredBuffer.h"
#include "DXBuffer.h"
#include "DX.h"
#include "DXRenderContext.h"

DXStructuredBuffer::DXStructuredBuffer()
{
}

DXStructuredBuffer::~DXStructuredBuffer()
{
	delete m_upload;
	delete m_commited;
}

void DXStructuredBuffer::Init(u32 count, u32 stride, void * data)
{
	m_upload = new DXBuffer;
	m_upload->Init(D3D12_HEAP_TYPE_UPLOAD, count * stride);

	assert(data);	// Data can't be null for static init
	m_commited = new DXBuffer;
	m_commited->Init(D3D12_HEAP_TYPE_DEFAULT, count * stride);

	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.NumElements = count;
	desc.Buffer.StructureByteStride = stride;
	desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	m_srvDesc = desc;

	m_srv.Create(m_commited->GetResource(), &desc);

	DX::Device->GetCopyableFootprints(&m_upload->GetDesc(), 0, 1, 0, &m_placedFootprint, &m_rowCount, &m_pitchInBytes, &m_totalBytes);

	memcpy(Map(), data,  count * stride);
	Unmap();
}

void DXStructuredBuffer::Unmap()
{
	DX::Uploader->RequestUpload(this);
}

u8 * DXStructuredBuffer::Map()
{
	return m_upload->Map();
}

void DXStructuredBuffer::Upload(DXRenderContext * rc)
{
	assert(m_commited);
	rc->CopyBufferRegion(m_commited->GetResource(), 0, m_upload->GetResource(), 0, m_totalBytes);
	rc->ResourceBarrier(m_commited->GetResource(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
}
