#include "stdafx.h"
#include "DXSRV.h"
#include "DX.h"
#include "DXDescriptorPool.h"

void DXRTV::Free()
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

void DXSRV::Free()
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
