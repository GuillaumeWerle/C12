#pragma once

class DXRootSignature
{
public:
	ComPtr<ID3D12RootSignature> m_rootSignature;
	u32 m_srvCount = 0;
	u32 m_uavCount = 0;

	void Create(u32 srvCount);

	DXRootSignature();
	~DXRootSignature();
};

