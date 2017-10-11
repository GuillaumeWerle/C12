#pragma once

#include "DXRootSignature.h"

class DXBuffer;

class Geometry
{
public:

    DXBuffer* m_streams[EVertexSteam_Count];

    void CreateVertexBuffer(u32 slot, u32 count, u32 stride, void * data);

    Geometry();
    ~Geometry();
};

