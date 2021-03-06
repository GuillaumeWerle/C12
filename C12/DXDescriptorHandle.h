#pragma once

class DXDescriptorHandle
{
public:

	DXDescriptorHandle() {}
	DXDescriptorHandle(CD3DX12_CPU_DESCRIPTOR_HANDLE _CPU, CD3DX12_GPU_DESCRIPTOR_HANDLE _GPU)
	{
		CPU = _CPU;
		GPU = _GPU;
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE CPU;
	CD3DX12_GPU_DESCRIPTOR_HANDLE GPU;
};
