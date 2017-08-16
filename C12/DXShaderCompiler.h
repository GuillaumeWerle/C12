#pragma once

#include "DXShader.h"

class DXShaderCompiler
{
public:

	DXShader * Compile(const std::wstring & path, DXShaderType shaderType, const std::string & entry);

	DXShaderCompiler();
	~DXShaderCompiler();
};

