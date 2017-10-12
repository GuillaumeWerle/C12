#include "stdafx.h"
#include "DXRootSignature.h"
#include <winerror.h>
#include "DX.h"
#include "ShaderCommon.h"
#include "Geometry.h"

DXRootSignature::DXRootSignature()
{
}

DXRootSignature::~DXRootSignature()
{
}

void DXRootSignature::Create(u32 srvCount)
{
	m_srvCount = srvCount;

	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = { D3D_ROOT_SIGNATURE_VERSION_1_1 };
	if (FAILED(DX::Device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

	CD3DX12_DESCRIPTOR_RANGE1 srvRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, srvCount, 0);
	//CD3DX12_DESCRIPTOR_RANGE1 vertexSrvRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, EVertexSteam_Count, 0, SPACE_VERTEX_SRV);

	std::array<CD3DX12_ROOT_PARAMETER1, ERootParamIndex_Count> rootParameters;
    rootParameters[ERootParamIndex_SRVTable].InitAsDescriptorTable(1, &srvRange, D3D12_SHADER_VISIBILITY_ALL);
    rootParameters[ERootParamIndex_CBGlobal].InitAsConstantBufferView(0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE);
    rootParameters[ERootParamIndex_CBEngineObj].InitAsConstantBufferView(0, SPACE_ENGINE_CB, D3D12_ROOT_DESCRIPTOR_FLAG_NONE);
    rootParameters[ERootParamIndex_CBEnginePass].InitAsConstantBufferView(1, SPACE_ENGINE_CB, D3D12_ROOT_DESCRIPTOR_FLAG_NONE);


	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_1((u32)rootParameters.size(), &rootParameters[0], 1, &sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	HRESULT hr;
	CHECK_D3DOK(hr, D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));
	CHECK_D3DOK(hr, DX::Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
}
