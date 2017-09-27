#pragma once

#include "DX.h"
#include "DXDescriptorHandle.h"

class DXSRV : public DXDescriptorHandle
{
public:

	DXSRV()
	{
		CPU.ptr = 0;
		GPU.ptr = 0;
	}
	
	~DXSRV()
	{
		Release();
	}

	void Release();

	void Create(ID3D12Resource * resource)
	{
		DXDescriptorHandle h = DX::PoolSRVCBVUAV->Alloc();
		DX::Device->CreateShaderResourceView(resource, nullptr, h.CPU);
		CPU = h.CPU;
		GPU = h.GPU;
	}
};

class DXRTV : public DXDescriptorHandle
{
public:

	DXRTV()
	{
		CPU.ptr = 0;
		GPU.ptr = 0;
	}

	~DXRTV()
	{
		Release();
	}

	void Release();

	void Create(ID3D12Resource * resource)
	{
		DXDescriptorHandle h = DX::PoolRTV->Alloc();
		DX::Device->CreateRenderTargetView(resource, nullptr, h.CPU);
		CPU = h.CPU;
		GPU = h.GPU;
	}
};

