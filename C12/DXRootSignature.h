#pragma once

enum ERootParamIndex
{
	SRVTable,
	CBGlobal,
	CBEngineObj,
	CBEnginePass,
	Count
};

class DXRootSignature
{
public:
	ComPtr<ID3D12RootSignature> m_rootSignature;
	u32 m_srvCount = 0;

	ID3D12RootSignature* Get() { return m_rootSignature.Get(); }
	void Create(u32 srvCount);

	DXRootSignature();
	~DXRootSignature();
};

