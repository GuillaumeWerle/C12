#pragma once

enum DXShaderType
{
	VS,
	PS,
};

class DXShader
{
public:
	ComPtr<ID3DBlob> m_blob;

	DXShader();
	~DXShader();
};

