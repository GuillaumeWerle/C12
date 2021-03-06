#include "stdafx.h"
#include "DXBuffer.h"
#include "DXBufferHeap.h"
#include "DX.h"
#include "DXRenderContext.h"

DXBuffer::DXBuffer()
{
    memset(&m_view, 0, sizeof(m_view));
}

DXBuffer::~DXBuffer()
{
    delete m_upload;
    delete m_commited;
}

void DXBuffer::Init(u32 count, u32 stride, void * data, EDXBufferUsage usage)
{
    m_usage = usage;

    m_commited = new DXBufferHeap;
    m_commited->Init(D3D12_HEAP_TYPE_DEFAULT, count * stride);

    m_upload = new DXBufferHeap;
    m_upload->Init(D3D12_HEAP_TYPE_UPLOAD, count * stride);
    DX::Device->GetCopyableFootprints(&m_upload->GetDesc(), 0, 1, 0, &m_placedFootprint, &m_rowCount, &m_pitchInBytes, &m_totalBytes);

    u8* dest = m_upload->Map();
    memcpy(dest, data, count * stride);
    DX::Uploader->RequestUpload(this);

    switch (usage)
    {
    case EDXBufferUsage_SRV:
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
        desc.Buffer.FirstElement = 0;
        desc.Buffer.NumElements = count;
        desc.Buffer.StructureByteStride = stride;
        desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
        desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        m_srv.Create(m_commited->GetResource(), &desc);
    }
    break;
    case EDXBufferUsage_VB:
        m_view.m_vbv.BufferLocation = m_commited->GetGPUVirtualAddress();
        m_view.m_vbv.SizeInBytes = stride * count;
        m_view.m_vbv.StrideInBytes = stride;
        break;
    case EDXBufferUsage_IB:
        assert(stride == sizeof(u32));  // only support 32bpp index 
        m_view.m_ibv.BufferLocation = m_commited->GetGPUVirtualAddress();
        m_view.m_ibv.Format = DXGI_FORMAT_R32_UINT;
        m_view.m_ibv.SizeInBytes = count * stride;
        break;
    default:
        assert(false);  // not handled
        break;
    }

}

void DXBuffer::Upload(DXRenderContext * rc)
{
    assert(m_commited);
    rc->CopyBufferRegion(m_commited->GetResource(), 0, m_upload->GetResource(), 0, m_totalBytes);
    rc->ResourceBarrier(m_commited->GetResource(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
}
