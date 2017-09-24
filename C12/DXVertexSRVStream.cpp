#include "stdafx.h"
#include "DXVertexSRVStream.h"
#include "DXBuffer.h"
#include "DX.h"
#include "DXRenderContext.h"

DXVertexSRVStream::DXVertexSRVStream()
{
}

DXVertexSRVStream::~DXVertexSRVStream()
{
	delete m_upload;
	delete m_commited;
}

void DXVertexSRVStream::Init(u32 count, u32 stride)
{
	m_upload = new DXBuffer;
	m_upload->Init(D3D12_HEAP_TYPE_UPLOAD, count * stride);

	m_commited = new DXBuffer;
	m_commited->Init(D3D12_HEAP_TYPE_DEFAULT, count * stride);

	m_srv = DX::PoolSRVCBVUAV->Alloc();
	
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.NumElements = count;
	desc.Buffer.StructureByteStride = stride;
	desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	m_srvDesc = desc;
	DX::Device->CreateShaderResourceView(m_commited->m_buffer.Get(), &desc, m_srv.CPU);
	DX::Device->GetCopyableFootprints(&m_upload->m_desc, 0, 1, 0, &m_placedFootprint, &m_rowCount, &m_pitchInBytes, &m_totalBytes);
}

u8 * DXVertexSRVStream::Map()
{
	return m_upload->m_cpuPtr;
}

void DXVertexSRVStream::Upload(DXRenderContext * rc)
{
	rc->CopyBufferRegion(m_commited->m_buffer.Get(), 0, m_upload->m_buffer.Get(), 0, m_totalBytes);
}
