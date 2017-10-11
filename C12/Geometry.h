#pragma once

class DXBuffer;
class DXRenderContext;

enum EVertexSteam
{
    EVertexSteam_Position,
    EVertexSteam_Normal,
    EVertexSteam_UV,
    EVertexSteam_Color,

    EVertexSteam_Count
};

class Geometry
{
public:

    DXBuffer* m_streams[EVertexSteam_Count];
    DXBuffer* m_ib;
    u32 m_indexCount = 0;

    void Bind(DXRenderContext* rc);

    void CreateVertexBuffer(u32 slot, u32 count, u32 stride, void * data);
    void CreateIndexBuffer(u32 count, u32 * data);

    Geometry();
    ~Geometry();
};

