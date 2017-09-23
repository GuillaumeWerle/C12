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
	u32 GetSrvCount() const { return m_srvCount; }
	ID3D12RootSignature* Get() { return m_rootSignature.Get(); }
	void Create(u32 srvCount);

	DXRootSignature();
	~DXRootSignature();

private:
	u32 m_srvCount = 0;
	ComPtr<ID3D12RootSignature> m_rootSignature;
};

