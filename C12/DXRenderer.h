// 15.08.2017
#pragma once

class DXBuffer;
class DXRootSignature;
class DXStructuredBuffer;

class DXRenderer
{
public:
	DXRootSignature* m_rootSignature;

	ComPtr<ID3D12PipelineState> m_pso;
	//ComPtr<ID3D12Resource> m_vertexBuffer;
	//D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	DXStructuredBuffer* m_streamPos;
	DXStructuredBuffer* m_streamUV;
	DXStructuredBuffer* m_streamColor;

	void Init();
	void Render(ComPtr<ID3D12GraphicsCommandList> & commandList);

	DXRenderer();
	~DXRenderer();
};

