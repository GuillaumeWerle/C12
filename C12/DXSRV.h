#pragma once

#include "DXDescriptorHandle.h"

class DXSRV : public DXDescriptorHandle
{
public:
	~DXSRV();
	void Create(ID3D12Resource * resource);
};

