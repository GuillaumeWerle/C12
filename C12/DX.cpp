#include "stdafx.h"
#include "DX.h"

DXDescriptorPool * DX::PoolSRVCBVUAV;
DXDescriptorPool * DX::PoolSampler;
DXDescriptorPool * DX::PoolRTV;
DXDescriptorPool * DX::PoolDSV;
DXUploader* DX::Uploader;
ID3D12Device * DX::Device;
