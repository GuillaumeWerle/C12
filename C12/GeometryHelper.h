// 11.10.2017

#pragma once

class Geometry;

namespace GeometryHelper
{
    enum ECreateUnitCube_CULLMODE
    {
        ECreateUnitCube_CULLMODE_FRONTFACE,
        ECreateUnitCube_CULLMODE_BACKFACE,
    };

    Geometry* CreateSphere(float sizeX, float sizeY, float sizeZ, unsigned int ySubdivisions, unsigned int rSubdivisions, bool facedir = false);
    Geometry* CreateUnitCube(ECreateUnitCube_CULLMODE cullMode = ECreateUnitCube_CULLMODE_FRONTFACE);
    Geometry* CreateUnitCubeWithNormalAndTexcoords();
    Geometry* CreateOctahedron(float radius);
    Geometry* CreateIcosahedron(float radius);
}
