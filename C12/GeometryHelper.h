// 11.10.2017

#pragma once

namespace GeometryHelper
{
    enum ECreateUnitCube_CULLMODE
    {
        ECreateUnitCube_CULLMODE_FRONTFACE,
        ECreateUnitCube_CULLMODE_BACKFACE,
    };

    //Geometry * CreateSphere(RenderInterface * renderInterface, float sizeX, float sizeY, float sizeZ, unsigned int ySubdivisions, unsigned int rSubdivisions, bool facedir = false);
    //Geometry * CreateUnitCube(RenderInterface * renderInterface, ECreateUnitCube_CULLMODE cullMode = ECreateUnitCube_CULLMODE_FRONTFACE);
    //Geometry * CreateUnitCubeWithNormalAndTexcoords(RenderInterface * renderInterface);
    //Geometry * CreateOctahedron(RenderInterface * renderInterface, float radius);
    //Geometry * CreateIcosahedron(RenderInterface * renderInterface, float radius);
    //Geometry * CreateFromObj(const std::string & objfilename);
}
