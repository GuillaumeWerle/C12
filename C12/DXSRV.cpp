#include "stdafx.h"
#include "DXSRV.h"
#include "DX.h"
#include "DXDescriptorPool.h"

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
