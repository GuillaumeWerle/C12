#include "stdafx.h"
#include "DXRenderer.h"
#include "DXBuffer.h"
#include "DX.h"
#include "DXShader.h"
#include "DXShaderCompiler.h"
#include "DXRootSignature.h"
#include "DXStructuredBuffer.h"

DXRenderer::DXRenderer()
{
	m_rootSignature = nullptr;
	m_streamPos = nullptr;
	m_streamUV = nullptr;
	m_streamColor = nullptr;
}

DXRenderer::~DXRenderer()
{
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

	// Define the vertex input layout.
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	// Create the vertex buffer.
	{
		struct Vertex
		{
			XMFLOAT3 position;
			XMFLOAT2 uv;
			XMFLOAT4 color;
		};

		// Define the geometry for a triangle.
		Vertex triangleVertices[] =
		{
			{ { 0.0f, 0.25f, 0.0f }, {0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.25f, -0.25f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.25f, -0.25f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};

		const UINT vertexBufferSize = sizeof(triangleVertices);

		// Note: using upload heaps to transfer static data like vert buffers is not 
		// recommended. Every time the GPU needs it, the upload heap will be marshalled 
		// over. Please read up on Default Heap usage. An upload heap is used here for 
		// code simplicity and because there are very few verts to actually transfer.
		//CHECK_D3DOK(hr, DX::Device->CreateCommittedResource(
		//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		//	D3D12_HEAP_FLAG_NONE,
		//	&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(&m_vertexBuffer)));

		// Copy the triangle data to the vertex buffer.
		//UINT8* pVertexDataBegin;
		//CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
		//CHECK_D3DOK(hr, m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
		//memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
		//m_vertexBuffer->Unmap(0, nullptr);

		//// Initialize the vertex buffer view.
		//m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		//m_vertexBufferView.StrideInBytes = sizeof(Vertex);
		//m_vertexBufferView.SizeInBytes = vertexBufferSize;

		XMFLOAT3 positions[] = { { 0.0f, 0.25f, 0.0f },{ 0.25f, -0.25f, 0.0f },{ -0.25f, -0.25f, 0.0f } };
		XMFLOAT2 uvs[] = { { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f } };
		XMFLOAT4 colors[] = { 
			{ 0.0f, 0.0f, 0.0f, 1.0f }, 
			{ 1.0f, 0.0f, 0.0f, 1.0f }, 
			{ 0.0f, 1.0f, 0.0f, 1.0f } };

		m_streamPos = new DXStructuredBuffer;
		m_streamUV = new DXStructuredBuffer;
		m_streamColor = new DXStructuredBuffer;

		m_streamPos->Init(_countof(positions), sizeof(XMFLOAT3), positions);
		m_streamUV->Init(_countof(uvs), sizeof(XMFLOAT2), uvs);
		m_streamColor->Init(_countof(colors), sizeof(XMFLOAT4), colors);
	}


	//	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	UINT compileFlags = 0;

	DXShader vs, ps;
	DXShaderCompiler compiler;
	compiler.Compile(vs, L"data\\shaders\\basic.hlsl", DXShaderType::VS, "VSMain");
	compiler.Compile(ps, L"data\\shaders\\basic.hlsl", DXShaderType::PS, "PSMain");

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
