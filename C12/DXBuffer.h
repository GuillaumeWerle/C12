#pragma once
#include "DXUploadable.h"
#include "DXDescriptorHandle.h"
#include "DXSRV.h"

class DXBufferHeap;

enum EDXBufferUsage
{
    EDXBufferUsage_SRV,
    EDXBufferUsage_VB,
    EDXBufferUsage_IB,
    
    EDXBufferUsage_UNKNOWN
};


class DXBuffer : public DXUploadable
{
public:
    const DXSRV & GetSRV() const { assert(m_usage == EDXBufferUsage_SRV); return m_srv; }
    D3D12_VERTEX_BUFFER_VIEW & GetVBV() { assert(m_usage == EDXBufferUsage_VB); return m_view.m_vbv; }
    D3D12_INDEX_BUFFER_VIEW & GetIBV() { assert(m_usage == EDXBufferUsage_IB); return m_view.m_ibv; }

    void Init(u32 count, u32 stride, void * data, EDXBufferUsage usage);

    DXBuffer();
    ~DXBuffer();

    virtual void Upload(DXRenderContext * rc) override;

private:
    EDXBufferUsage m_usage = EDXBufferUsage_UNKNOWN;
    DXBufferHeap* m_upload = nullptr;
    DXBufferHeap* m_commited = nullptr;
    DXSRV m_srv;
    union UView
    {
        D3D12_VERTEX_BUFFER_VIEW m_vbv;
        D3D12_INDEX_BUFFER_VIEW m_ibv;
    };
    UView m_view;
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT m_placedFootprint = {};
    u32 m_rowCount = 0;
    u64 m_pitchInBytes = 0;
    u64 m_totalBytes = 0;
};

