#include "stdafx.h"
#include "DXSRV.h"
#include "DX.h"
#include "DXDescriptorPool.h"

void DXSRV::Create(ID3D12Resource * resource, D3D12_SHADER_RESOURCE_VIEW_DESC * desc)
{
	DXDescriptorHandle h = DX::PoolSRVCBVUAV->Alloc();
	DX::Device->CreateShaderResourceView(resource, desc, h.CPU);
	CPU = h.CPU;
	GPU = h.GPU;
}

void DXSRV::Release()
{
	if (CPU.ptr)
	{
		DXDescriptorHandle h(CPU,GPU);
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
		DXDescriptorHandle h(CPU,GPU);
		DX::PoolRTV->Free(h);
		CPU.ptr = 0;
	}
}

void DXDSV::Create(ID3D12Resource * resource)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvdesc = {};
	dsvdesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvdesc.Texture2D.MipSlice = 0;

	DXDescriptorHandle h = DX::PoolDSV->Alloc();
	DX::Device->CreateDepthStencilView(resource, &dsvdesc, h.CPU);
	CPU = h.CPU;
	GPU = h.GPU;
}

void DXDSV::Release()
{
	if (CPU.ptr)
	{
		DXDescriptorHandle h(CPU, GPU);
		DX::PoolDSV->Free(h);
		CPU.ptr = 0;
	}
}
