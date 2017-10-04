#include "stdafx.h"
#include "DXSRV.h"
#include "DX.h"
#include "DXDescriptorPool.h"

void DXSRV::Create(ID3D12Resource * resource, D3D12_SHADER_RESOURCE_VIEW_DESC * desc)
{
	DXDescriptorHandle h = DX::PoolSRVCBVUAV->Alloc();
	DX::Device->CreateShaderResourceView(resource, nullptr, h.CPU);
	CPU = h.CPU;
	GPU = h.GPU;
}

void DXSRV::Release()
{
	if (CPU.ptr)
	{
		DXDescriptorHandle h;
		h.CPU = CPU;
		h.GPU = GPU;
		DX::PoolSRVCBVUAV->Free(h);
		CPU.ptr = 0;
	}
}

void DXRTV::Create(ID3D12Resource * resource)
{
	DXDescriptorHandle h = DX::PoolRTV->Alloc();
	DX::Device->CreateRenderTargetView(resource, nullptr, h.CPU);
	CPU = h.CPU;
	GPU = h.GPU;
}

void DXRTV::Release()
{
	if (CPU.ptr)
	{
		DXDescriptorHandle h;
		h.CPU = CPU;
		h.GPU = GPU;
		DX::PoolRTV->Free(h);
		CPU.ptr = 0;
	}
}
