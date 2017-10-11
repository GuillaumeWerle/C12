#pragma once
#include "DXUploadable.h"
#include "DXDescriptorHandle.h"
#include "DXSRV.h"

class DXBufferHeap;

enum EDXBufferUsage
{
    EDXBufferUsage_VB,
    EDXBufferUsage_SRV,
};

class DXBuffer : public DXUploadable
{
public:
    const DXSRV & GetSRV() const { return m_srv; }
    D3D12_VERTEX_BUFFER_VIEW & GetVBV() { return m_vbv; }

    void Init(u32 count, u32 stride, void * data, EDXBufferUsage usage);

    DXBuffer();
    ~DXBuffer();

    virtual void Upload(DXRenderContext * rc) override;

private:
    DXBufferHeap* m_upload = nullptr;
    DXBufferHeap* m_commited = nullptr;
    DXSRV m_srv;
    D3D12_VERTEX_BUFFER_VIEW m_vbv;
    D3D12_SHADER_RESOURCE_VIEW_DESC m_srvDesc = {};
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT m_placedFootprint = {};
    u32 m_rowCount = 0;
    u64 m_pitchInBytes = 0;
    u64 m_totalBytes = 0;
};

