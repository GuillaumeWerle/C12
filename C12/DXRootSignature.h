#pragma once

enum class ERootParamIndex : std::uint32_t
{
	SRVTable,
	CBGlobal,
	CBEngineObj,
	CBEnginePass,
	SRVVertexStreamsTable,
	Count
};

enum class EVertexSteam : std::uint32_t
{
	Position,
	UV,
	Color,
	Count
};

class DXRootSignature
{
public:
	ID3D12RootSignature* Get() { return m_rootSignature.Get(); }

	u32 GetSRVCount() const { return m_srvCount; }

	void Create(u32 srvCount);

	DXRootSignature();
	~DXRootSignature();

private:
	u32 m_srvCount = 0;
	ComPtr<ID3D12RootSignature> m_rootSignature;
};

