#include "stdafx.h"
#include "DXSRV.h"
#include "DX.h"
#include "DXDescriptorPool.h"

DXSRV::~DXSRV()
{
	DXDescriptorHandle h;
	h.CPU = CPU;
	h.GPU = GPU;
	DX::PoolSRVCBVUAV->Free(h);
}

void DXSRV::Create(ID3D12Resource * resource)
{
	DXDescriptorHandle h = DX::PoolSRVCBVUAV->Alloc();
	DX::Device->CreateShaderResourceView(resource, nullptr, h.CPU);
	CPU = h.CPU;
	GPU = h.GPU;
}
