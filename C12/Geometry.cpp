#include "stdafx.h"
#include "Geometry.h"
#include "DXBuffer.h"
#include "DXRenderContext.h"

Geometry::Geometry()
{
    m_ib = nullptr;
    for (auto & it : m_streams)
        it = nullptr;
}

Geometry::~Geometry()
{
    delete m_ib;
    for (auto & it : m_streams)
        delete it;
}

void Geometry::CreateVertexBuffer(u32 slot, u32 count, u32 stride, void * data)
{
    assert(m_streams[slot] == nullptr);
    m_streams[slot] = new DXBuffer;
    m_streams[slot]->Init(count, stride, data, EDXBufferUsage_VB);
}

void Geometry::CreateIndexBuffer(u32 count, u32 * data)
{
    assert(m_ib);
    m_ib = new DXBuffer;
    m_ib->Init(count, sizeof(u32), data, EDXBufferUsage_IB);
    m_indexCount = count;
}

void Geometry::Bind(DXRenderContext* rc)
{
    for (u32 i = 0; i < EVertexSteam_Count; i++)
        rc->SetVertexBuffer(i, &m_streams[i]->GetVBV());

    rc->SetIndexBuffer(&m_ib->GetIBV());
}
