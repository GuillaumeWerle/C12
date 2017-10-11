#pragma once

enum ERootParamIndex
{
    ERootParamIndex_SRVTable,
    ERootParamIndex_CBGlobal,
    ERootParamIndex_CBEngineObj,
    ERootParamIndex_CBEnginePass,

    ERootParamIndex_Count
};

enum EVertexSteam
{
    EVertexSteam_Position,
    EVertexSteam_UV,
    EVertexSteam_Color,

    EVertexSteam_Count
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

