#pragma once

#include "DXDescriptorPool.h"
#include "DXDescriptorHeapLinear.h"

class DX
{
public:
	static DXDescriptorPool * PoolSRVCBVUAV;
	static DXDescriptorPool * PoolSampler;
	static DXDescriptorPool * PoolRTV;
	static DXDescriptorPool * PoolDSV;
	static ID3D12Device * Device;
};

