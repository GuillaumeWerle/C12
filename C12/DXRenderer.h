// 15.08.2017
#pragma once

class DXBufferHeap;
class DXRootSignature;
class DXBuffer;

class DXRenderer
{
public:
	DXRootSignature* m_rootSignature;

	ComPtr<ID3D12PipelineState> m_pso;
	//ComPtr<ID3D12Resource> m_vertexBuffer;
	//D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	DXBuffer* m_streamPos;
	DXBuffer* m_streamUV;
	DXBuffer* m_streamColor;
    DXBuffer* m_indexBuffer;

	void Init();
	void Render(ComPtr<ID3D12GraphicsCommandList> & commandList);

	DXRenderer();
	~DXRenderer();
};

