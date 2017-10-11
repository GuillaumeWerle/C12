#include "stdafx.h"
#include "Geometry.h"
#include "DXBuffer.h"

Geometry::Geometry()
{
    for (auto & it : m_streams)
        it = nullptr;
}


Geometry::~Geometry()
{
    for (auto & it : m_streams)
        delete it;
}

void Geometry::CreateVertexBuffer(u32 slot, u32 count, u32 stride, void * data)
{
    assert(m_streams[slot] == nullptr);
    m_streams[slot] = new DXBuffer;
    m_streams[slot]->Init(count, stride, data, EDXBufferUsage_VB);
}

