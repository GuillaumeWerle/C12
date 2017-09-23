#pragma once

#include "DXDescriptorPool.h"
#include "DXDescriptorHeapLinear.h"
#include "DXUploader.h"

class DX
{
public:
	static ID3D12Device* Device;
	static DXDescriptorPool* PoolSRVCBVUAV;
	static DXDescriptorPool* PoolSampler;
	static DXDescriptorPool* PoolRTV;
	static DXDescriptorPool* PoolDSV;
	static DXUploader* Uploader;
};

