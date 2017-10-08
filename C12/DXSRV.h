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

	void Create(ID3D12Resource * resource, D3D12_SHADER_RESOURCE_VIEW_DESC * desc);
	void Release();
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

	void Create(ID3D12Resource * resource);
	void Release();
};

class DXDSV : public DXDescriptorHandle
{
public:

	DXDSV()
	{
		CPU.ptr = 0;
		GPU.ptr = 0;
	}

	~DXDSV()
	{
		Release();
	}

	void Create(ID3D12Resource * resource);
	void Release();
};

