//  [13/10/2012 Guillaume]
#include "stdafx.h"
#include "GeometryHelper.h"
#include "Geometry.h"

#pragma warning( disable : 4305 )	// 'initializing' : truncation from 'double' to 'float'

#if 0

namespace GeometryHelper
{
    static float *s_addVertRing(float *vertexData, unsigned int subDiv, float sizeX, float posY, float sizeZ)
    {
        float rDiv = 6.2831853f / (float)subDiv;

        for (unsigned int r = 0; r <= subDiv; r++)
        {
            float rAng = (float)r;
            if (r == subDiv)
                rAng = 0;
            rAng *= rDiv;
            *vertexData++ = sinf(rAng) * sizeX;
            *vertexData++ = posY;
            *vertexData++ = cosf(rAng) * sizeZ;
        }

        return vertexData;
    }

    static float *s_addStRing(float *stData, unsigned int subDiv, float t)
    {
        for (unsigned int r = 0; r <= subDiv; r++)
        {
            *stData++ = (float)r / subDiv;
            *stData++ = t;
        }

        return stData;
    }

    Geometry * CreateSphere(RenderInterface * renderInterface, float sizeX, float sizeY, float sizeZ, unsigned int ySubdivisions, unsigned int rSubdivisions, bool facedir /*= true*/)
    {
        unsigned int i, r;

        // This is slightly inefficient - the top and bottom row will be half degenerate.
        unsigned int polyCount = (rSubdivisions * (ySubdivisions - 2) * 2)	// Middle section has quads split into tris
            + (rSubdivisions * 2);						// Top and bottom has triangles for pinch.
        unsigned int indexCount = polyCount * 3;
        unsigned int vertexCount = ((ySubdivisions + 1) * (rSubdivisions + 1));

        if (0 == sizeY) sizeY = sizeX;
        if (0 == sizeZ) sizeZ = sizeX;

        std::vector<float> vposition(vertexCount * 3);
        std::vector<float> vnormal(vertexCount * 3);
        std::vector<float> vtexcoord(vertexCount * 2);
        std::vector<uint> vindex(indexCount);


        float *vertexData = (float *)&vposition[0];
        float *normalData = (float *)&vnormal[0];
        float *stData = (float *)&vtexcoord[0];
        uint *indexData = (uint *)&vindex[0];

        for (i = 0; i < ySubdivisions; i++)
        {
            for (r = 0; r < rSubdivisions; r++)
            {
                // Top left of quad
                unsigned int tl = (i * (rSubdivisions + 1)) + r;
                unsigned int tr = tl + 1;
                unsigned int bl = tl + rSubdivisions + 1;
                unsigned int br = bl + 1;

                if (i == 0)
                {
                    *indexData++ = (uint)tl;
                    *indexData++ = (uint)bl;
                    *indexData++ = (uint)br;
                }
                else if (i == (ySubdivisions - 1))
                {
                    *indexData++ = (uint)tr;
                    *indexData++ = (uint)tl;
                    *indexData++ = (uint)br;
                }
                else
                {
                    *indexData++ = (uint)tr;
                    *indexData++ = (uint)tl;
                    *indexData++ = (uint)br;

                    *indexData++ = (uint)tl;
                    *indexData++ = (uint)bl;
                    *indexData++ = (uint)br;
                }
            }
        }

        if (facedir)
            for (uint f = 0; f < vindex.size(); f += 3)
                std::swap(vindex[f + 0], vindex[f + 1]);

        //PSSG_ASSERT(indexData == ((unsigned short *)indexSource->getData() + indexCount));

        // Add a data block to contain the vertices
        for (i = 0; i <= rSubdivisions; i++)
        {
            *vertexData++ = 0.0f;
            *vertexData++ = sizeY;
            *vertexData++ = 0.0f;
        }
        for (i = 1; i < ySubdivisions; i++)
        {
            float y = cosf((i * (float)COCOON_PI) / ySubdivisions);
            float xs = sqrtf(1.0f - y*y);

            vertexData = s_addVertRing(vertexData, rSubdivisions, sizeX*xs, sizeY*y, sizeZ*xs);
        }
        for (i = 0; i <= rSubdivisions; i++)
        {
            *vertexData++ = 0.0f;
            *vertexData++ = -sizeY;
            *vertexData++ = 0.0f;
        }

        // Add a data block to contain the normals
        for (i = 0; i <= ySubdivisions; i++)
        {
            float y = cosf((i * (float)COCOON_PI) / (ySubdivisions));
            float xs = sqrtf(1.0f - y*y);
            for (r = 0; r <= rSubdivisions; r++)
            {
                *normalData++ = sinf((r * (float)COCOON_PI * 2) / rSubdivisions) * xs;
                *normalData++ = y;
                *normalData++ = cosf((r * (float)COCOON_PI * 2) / rSubdivisions) * xs;
            }
        }

        // Add a data block to contain the STs
        for (i = 0; i <= ySubdivisions; i++)
        {
            float y = (float)i / ySubdivisions;
            stData = s_addStRing(stData, rSubdivisions, y);
        }

        ID3D11Device * device = renderInterface->GetD3DDevice();

        Geometry * geom = new Geometry;

        HRESULT hr;
        CHECK_OK(hr, geom->CreateVertexBuffer(device, EShaderInputSlot_POSITION, (unsigned int)vposition.size(), sizeof(float), &vposition[0]));
        CHECK_OK(hr, geom->CreateVertexBuffer(device, EShaderInputSlot_NORMAL, (unsigned int)vnormal.size(), sizeof(float), &vnormal[0]));
        CHECK_OK(hr, geom->CreateVertexBuffer(device, EShaderInputSlot_TEXCOORD, (unsigned int)vtexcoord.size(), sizeof(float), &vtexcoord[0]));
        CHECK_OK(hr, geom->CreateIndexBuffer((uint)vindex.size(), &vindex[0]));

        return geom;
    }

    Geometry * CreateUnitCube(RenderInterface * renderInterface, ECreateUnitCube_CULLMODE cullMode)
    {
        unsigned int vertexCount = 8;
        unsigned indexCount = 6 * 2 * 3;

        float cube[] =
        {
            -1.0f, -1.0f, -1.0f,
            +1.0f, -1.0f, -1.0f,
            -1.0f, +1.0f, -1.0f,
            +1.0f, +1.0f, -1.0f,
            -1.0f, -1.0f, +1.0f,
            +1.0f, -1.0f, +1.0f,
            -1.0f, +1.0f, +1.0f,
            +1.0f, +1.0f, +1.0f,
        };

        unsigned int index[] =
        {
            0, 2, 3,
            3, 1, 0,
            4, 5, 7,
            7, 6, 4,
            0, 1, 5,
            5, 4, 0,
            1, 3, 7,
            7, 5, 1,
            3, 2, 6,
            6, 7, 3,
            2, 0, 4,
            4, 6, 2,
        };

        if (cullMode == ECreateUnitCube_CULLMODE_BACKFACE)
        {
            for (uint i = 0; i < _countof(index); i += 3)
            {
                unsigned short tmp = index[i + 1];
                index[i + 1] = index[i + 2];
                index[i + 2] = tmp;
            }
        }

        Geometry * geom = new Geometry;

        HRESULT hr = 0;

        ID3D11Device * device = renderInterface->GetD3DDevice();
        COCOON_OK_NO_RETURN(hr, geom->CreateVertexBuffer(device, EShaderInputSlot_POSITION, _countof(cube) / 3, 12, cube));
        COCOON_OK_NO_RETURN(hr, geom->CreateIndexBuffer(_countof(index), index));

        return geom;
    }

    Geometry * CreateOctahedron(RenderInterface * renderInterface, float radius)
    {

        float p[] =
        {
            0, 0, 1,
            1, 0, 0,
            0, 1, 0,
            -1, 0, 0,
            0, -1, 0,
            0, 0, -1,
        };

        unsigned int f[] =
        {
            1, 0, 2,
            2, 0, 3,
            3, 0, 4,
            4, 0, 1,
            2, 5, 1,
            3, 5, 2,
            4, 5, 3,
            1, 5, 4,
        };

        ID3D11Device * device = renderInterface->GetD3DDevice();
        Geometry * geom = new Geometry;

        HRESULT hr = S_OK;
        COCOON_OK_NO_RETURN(hr, geom->CreateVertexBuffer(device, EShaderInputSlot_NORMAL, _countof(p) / 3, 12, p));

        for (unsigned int i = 0; i < _countof(p); i++)
            p[i] *= radius;

        COCOON_OK_NO_RETURN(hr, geom->CreateVertexBuffer(device, EShaderInputSlot_NORMAL, _countof(p) / 3, 12, p));
        COCOON_OK_NO_RETURN(hr, geom->CreateIndexBuffer(_countof(f), f));

        return geom;
    }

    Geometry * CreateIcosahedron(RenderInterface * renderInterface, float radius)
    {
        // icosahedron tesselated (level 2)
        // cut and paste from an .obj file

        float p[] =
        {
            0.0000f, 0.0000f, 0.5000f,
            0.4472f, 0.0000f, 0.2236f,
            0.1382f, 0.4253f, 0.2236f,
            -0.3618f, 0.2629f, 0.2236f,
            -0.3618f, -0.2629f, 0.2236f,
            0.1382f, -0.4253f, 0.2236f,
            0.3618f, 0.2629f, -0.2236f,
            -0.1382f, 0.4253f, -0.2236f,
            -0.4472f, -0.0000f, -0.2236f,
            -0.1382f, -0.4253f, -0.2236f,
            0.3618f, -0.2629f, -0.2236f,
            0.0000f, 0.0000f, -0.5000f,
            0.2629f, 0.0000f, 0.4253f,
            0.0812f, 0.2500f, 0.4253f,
            -0.2127f, 0.1545f, 0.4253f,
            -0.2127f, -0.1545f, 0.4253f,
            0.0812f, -0.2500f, 0.4253f,
            0.3441f, 0.2500f, 0.2629f,
            -0.1314f, 0.4045f, 0.2629f,
            -0.4253f, -0.0000f, 0.2629f,
            -0.1314f, -0.4045f, 0.2629f,
            0.3441f, -0.2500f, 0.2629f,
            0.4755f, 0.1545f, 0.0000f,
            -0.0000f, 0.5000f, 0.0000f,
            -0.4755f, 0.1545f, 0.0000f,
            -0.2939f, -0.4045f, 0.0000f,
            0.2939f, -0.4045f, 0.0000f,
            0.4755f, -0.1545f, 0.0000f,
            0.2939f, 0.4045f, 0.0000f,
            -0.2939f, 0.4045f, 0.0000f,
            -0.4755f, -0.1545f, 0.0000f,
            0.0000f, -0.5000f, 0.0000f,
            0.1314f, 0.4045f, -0.2629f,
            -0.3441f, 0.2500f, -0.2629f,
            -0.3441f, -0.2500f, -0.2629f,
            0.1314f, -0.4045f, -0.2629f,
            0.4253f, 0.0000f, -0.2629f,
            0.2127f, 0.1545f, -0.4253f,
            -0.0812f, 0.2500f, -0.4253f,
            -0.2629f, -0.0000f, -0.4253f,
            -0.0812f, -0.2500f, -0.4253f,
            0.2127f, -0.1545f, -0.4253f,
        };

        unsigned int f[] =
        {
            1, 13, 14,
            13, 2, 18,
            13, 18, 14,
            14, 18, 3,
            1, 14, 15,
            14, 3, 19,
            14, 19, 15,
            15, 19, 4,
            1, 15, 16,
            15, 4, 20,
            15, 20, 16,
            16, 20, 5,
            1, 16, 17,
            16, 5, 21,
            16, 21, 17,
            17, 21, 6,
            1, 17, 13,
            17, 6, 22,
            17, 22, 13,
            13, 22, 2,
            2, 28, 23,
            28, 11, 37,
            28, 37, 23,
            23, 37, 7,
            3, 29, 24,
            29, 7, 33,
            29, 33, 24,
            24, 33, 8,
            4, 30, 25,
            30, 8, 34,
            30, 34, 25,
            25, 34, 9,
            5, 31, 26,
            31, 9, 35,
            31, 35, 26,
            26, 35, 10,
            6, 32, 27,
            32, 10, 36,
            32, 36, 27,
            27, 36, 11,
            7, 29, 23,
            29, 3, 18,
            29, 18, 23,
            23, 18, 2,
            8, 30, 24,
            30, 4, 19,
            30, 19, 24,
            24, 19, 3,
            9, 31, 25,
            31, 5, 20,
            31, 20, 25,
            25, 20, 4,
            10, 32, 26,
            32, 6, 21,
            32, 21, 26,
            26, 21, 5,
            11, 28, 27,
            28, 2, 22,
            28, 22, 27,
            27, 22, 6,
            12, 39, 38,
            39, 8, 33,
            39, 33, 38,
            38, 33, 7,
            12, 40, 39,
            40, 9, 34,
            40, 34, 39,
            39, 34, 8,
            12, 41, 40,
            41, 10, 35,
            41, 35, 40,
            40, 35, 9,
            12, 42, 41,
            42, 11, 36,
            42, 36, 41,
            41, 36, 10,
            12, 38, 42,
            38, 7, 37,
            38, 37, 42,
            42, 37, 11,
        };

        for (unsigned int i = 0; i < _countof(f); i += 3)
        {
            f[i + 0] = f[i + 0] - 1;
            f[i + 1] = f[i + 1] - 1;
            f[i + 2] = f[i + 2] - 1;

            std::swap(f[i + 1], f[i + 2]);
        }

        ID3D11Device * device = renderInterface->GetD3DDevice();
        Geometry * geom = new Geometry;

        HRESULT hr = S_OK;
        COCOON_OK_NO_RETURN(hr, geom->CreateVertexBuffer(device, EShaderInputSlot_NORMAL, _countof(p) / 3, 12, p));

        for (unsigned int i = 0; i < _countof(p); i++)
            p[i] *= radius;

        COCOON_OK_NO_RETURN(hr, geom->CreateVertexBuffer(device, EShaderInputSlot_NORMAL, _countof(p) / 3, 12, p));
        COCOON_OK_NO_RETURN(hr, geom->CreateIndexBuffer(_countof(f), f));

        return geom;
    }

    const char * nextline(const char * txt)
    {
        const char * end = strstr(txt, "\n") + 1;
        if (*end == 0)
            return nullptr;
        else
            return end;
    }

    struct ObjVertex
    {
        float3 pos;
        float3 norm;
        float2 uv;

        bool operator == (const ObjVertex & _Src) const
        {
            const bool br = memcmp(this, &_Src, sizeof(*this)) == 0;
            return br;
        }

        bool operator < (const ObjVertex & _Src) const
        {
            const bool br = memcmp(this, &_Src, sizeof(*this)) < 0;
            return br;
        }
    };

    COCOON_API Geometry * CreateFromObj(const std::string & objfilename)
    {
        SharedPtr<FileBuffer> fb = FileSystem::ms_instance->ReadFile(objfilename);

        float x, y, z;
        uint a[3];
        uint b[3];
        uint c[3];

        std::vector<float3> pos;
        std::vector<float3> normal;
        std::vector<float2> uv;
        std::vector<uint> idx;

        impMapArray<ObjVertex> mesh;

        const char * txt = (const char *)fb->GetData();
        for (; txt; txt = nextline(txt))
        {
            const char * line = txt;
            if (StringStartsWith(line, "# "))	// comment
                continue;

            if (StringStartsWith(line, "v "))	// vertex
            {
                sscanf(txt, "v %f %f %f", &x, &y, &z);
                pos.push_back(float3(x, y, z));
            }
            if (StringStartsWith(line, "vn "))	// vertex
            {
                sscanf(txt, "vn %f %f %f", &x, &y, &z);
                normal.push_back(float3(x, y, z));
            }
            if (StringStartsWith(line, "vt "))	// vertex
            {
                sscanf(txt, "vt %f %f", &x, &y);
                uv.push_back(float2(x, y));
            }
            if (StringStartsWith(line, "f "))	// face
            {
                sscanf(txt, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                    &a[0], &a[1], &a[2],
                    &b[0], &b[1], &b[2],
                    &c[0], &c[1], &c[2]);

                ObjVertex v;
                v.pos = pos[a[0] - 1];
                v.norm = normal[a[2] - 1];
                v.uv = uv[a[1] - 1];
                idx.push_back((uint)mesh[v]);
                v.pos = pos[b[0] - 1];
                v.norm = normal[b[2] - 1];
                v.uv = uv[b[1] - 1];
                idx.push_back((uint)mesh[v]);
                v.pos = pos[c[0] - 1];
                v.norm = normal[c[2] - 1];
                v.uv = uv[c[1] - 1];
                idx.push_back((uint)mesh[v]);
            }
        }

        pos.clear();
        normal.clear();
        uv.clear();

        for (uint i = 0; i < mesh.m_map.size(); i++)
        {
            const ObjVertex & vtx = mesh[i];
            pos.push_back(vtx.pos);
            normal.push_back(vtx.norm);
            uv.push_back(vtx.uv);
        }

        ID3D11Device * device = g_D3DDevice;
        Geometry * geom = new Geometry;

        HRESULT hr = S_OK;
        COCOON_OK_NO_RETURN(hr, geom->CreateVertexBuffer(device, EShaderInputSlot_POSITION, (uint)pos.size(), 12, &pos[0]));
        COCOON_OK_NO_RETURN(hr, geom->CreateVertexBuffer(device, EShaderInputSlot_NORMAL, sizeof(normal[0]), (uint)normal.size(), &normal[0]));
        COCOON_OK_NO_RETURN(hr, geom->CreateVertexBuffer(device, EShaderInputSlot_TEXCOORD, sizeof(uv[0]), (uint)uv.size(), &uv[0]));
        COCOON_OK_NO_RETURN(hr, geom->CreateIndexBuffer((uint)idx.size(), &idx[0]));

        return geom;

    }

    Geometry * CreateUnitCubeWithNormalAndTexcoords(RenderInterface * renderInterface)
    {

        float pos[] =
        {
            -1.0f, -1.0f, -1.0f,
            +1.0f, +1.0f, -1.0f,
            -1.0f, +1.0f, -1.0f,
            +1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, +1.0f,
            +1.0f, +1.0f, +1.0f,
            +1.0f, -1.0f, +1.0f,
            -1.0f, +1.0f, +1.0f,
            -1.0f, -1.0f, -1.0f,
            +1.0f, -1.0f, +1.0f,
            +1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, +1.0f,
            +1.0f, -1.0f, -1.0f,
            +1.0f, +1.0f, +1.0f,
            +1.0f, +1.0f, -1.0f,
            +1.0f, -1.0f, +1.0f,
            +1.0f, +1.0f, -1.0f,
            -1.0f, +1.0f, +1.0f,
            -1.0f, +1.0f, -1.0f,
            +1.0f, +1.0f, +1.0f,
            -1.0f, +1.0f, -1.0f,
            -1.0f, -1.0f, +1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, +1.0f, +1.0f,
        };

        float norm[] =
        {
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, +1,
            0, 0, +1,
            0, 0, +1,
            0, 0, +1,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            +1, 0, 0,
            +1, 0, 0,
            +1, 0, 0,
            +1, 0, 0,
            0, +1, 0,
            0, +1, 0,
            0, +1, 0,
            0, +1, 0,
            -1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,
        };

        float uv[] =
        {
            -1, 0,
            -1, 0,
            -1, 0,
            -1, 0,
            +1, 0,
            +1, 0,
            +1, 0,
            +1, 0,
            +1, 0,
            +1, 0,
            +1, 0,
            +1, 0,
            0, +1,
            0, +1,
            0, +1,
            0, +1,
            -1, 0,
            -1, 0,
            -1, 0,
            -1, 0,
            0, -1,
            0, -1,
            0, -1,
            0, -1,
        };

        unsigned int faces[] =
        {
            0, 1, 2,
            1, 0, 3,
            4, 5, 6,
            5, 4, 7,
            8, 9, 10,
            9, 8, 11,
            12, 13, 14,
            13, 12, 15,
            16, 17, 18,
            17, 16, 19,
            20, 21, 22,
            21, 20, 23,
        };

        ID3D11Device * device = renderInterface->GetD3DDevice();
        Geometry * geom = new Geometry;

        HRESULT hr = S_OK;
        COCOON_OK_NO_RETURN(hr, geom->CreateVertexBuffer(device, EShaderInputSlot_POSITION, _countof(pos) / 3, 12, pos));
        COCOON_OK_NO_RETURN(hr, geom->CreateVertexBuffer(device, EShaderInputSlot_NORMAL, _countof(norm) / 3, 12, norm));
        COCOON_OK_NO_RETURN(hr, geom->CreateVertexBuffer(device, EShaderInputSlot_TEXCOORD, _countof(uv) / 2, 8, uv));
        COCOON_OK_NO_RETURN(hr, geom->CreateIndexBuffer(_countof(faces), faces));

        return geom;
    }

}


#endif // 0