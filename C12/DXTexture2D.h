#pragma once

class DXTexture2D
{
public:
	ComPtr<ID3D12Resource> m_resource;
	ComPtr<ID3D12Resource> m_uploadBuffer;

	void Init(ComPtr<ID3D12GraphicsCommandList> & commandList);

	DXTexture2D();
	~DXTexture2D();
};

