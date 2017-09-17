#pragma once

#include "DXShader.h"

class DXShaderCompiler
{
public:

	HRESULT Compile(DXShader & output, const std::wstring & path, DXShaderType shaderType, const std::string & entry);

	DXShaderCompiler();
	~DXShaderCompiler();
};

