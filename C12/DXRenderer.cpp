#include "stdafx.h"
#include "DXRenderer.h"
#include "DXBufferHeap.h"
#include "DX.h"
#include "DXShader.h"
#include "DXShaderCompiler.h"
#include "DXRootSignature.h"
#include "DXBuffer.h"

DXRenderer::DXRenderer()
{
	m_rootSignature = nullptr;
	m_streamPos = nullptr;
	m_streamUV = nullptr;
	m_streamColor = nullptr;
    m_indexBuffer = nullptr;
}

DXRenderer::~DXRenderer()
{
    delete m_indexBuffer;
	delete m_streamColor;
	delete m_streamUV;
	delete m_streamPos;
	delete m_rootSignature;
}

void DXRenderer::Init()
{
	HRESULT hr = 0;

	// Create the root signature.
	m_rootSignature = new DXRootSignature;
	m_rootSignature->Create(16);

	{
		XMFLOAT3 positions[] = { { 0.0f, 0.25f, 0.0f },{ 0.25f, -0.25f, 0.0f },{ -0.25f, -0.25f, 0.0f } };
		XMFLOAT2 uvs[] = { { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f } };
		XMFLOAT4 colors[] = { 
			{ 0.0f, 0.0f, 1.0f, 1.0f }, 
			{ 1.0f, 0.0f, 0.0f, 1.0f }, 
			{ 0.0f, 1.0f, 0.0f, 1.0f } };

		m_streamPos = new DXBuffer;
		m_streamUV = new DXBuffer;
		m_streamColor = new DXBuffer;

		m_streamPos->Init(_countof(positions), sizeof(XMFLOAT3), positions, EDXBufferUsage_VB);
		m_streamUV->Init(_countof(uvs), sizeof(XMFLOAT2), uvs, EDXBufferUsage_VB);
		m_streamColor->Init(_countof(colors), sizeof(XMFLOAT4), colors, EDXBufferUsage_VB);

        u32 ib[] = { 0, 1, 2 };
        m_indexBuffer = new DXBuffer;
        m_indexBuffer->Init(_countof(ib), sizeof(u32), ib, EDXBufferUsage_IB);
	}

	//	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	UINT compileFlags = 0;

	DXShader vs, ps;
	DXShaderCompiler compiler;
	compiler.Compile(vs, L"data\\shaders\\basic.hlsl", DXShaderType::VS, "VSMain", std::vector<DXMacro>());
	compiler.Compile(ps, L"data\\shaders\\basic.hlsl", DXShaderType::PS, "PSMain", std::vector<DXMacro>());

    // Define the vertex input layout.
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

	// Describe and create the graphics pipeline state object (PSO).
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_rootSignature->Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(vs.m_blob.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(ps.m_blob.Get());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	CHECK_D3DOK(hr, DX::Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pso)));
}

void DXRenderer::Render(ComPtr<ID3D12GraphicsCommandList> & commandList)
{

}
