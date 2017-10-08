#pragma once

#include "DXShader.h"
#include "DXMacro.h"

class DXShaderCompiler
{
public:

	HRESULT Compile(DXShader & output, const std::wstring & path, DXShaderType shaderType, const std::string & entry, const std::vector<DXMacro> & macros);

	DXShaderCompiler();
	~DXShaderCompiler();
};

