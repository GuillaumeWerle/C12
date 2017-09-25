#include "stdafx.h"
#include "DXShaderCompiler.h"

#pragma comment (lib, "d3dcompiler.lib")

class DXShaderIncludeInterface : public ID3DInclude
{
public:

    HRESULT Open(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
    {
        //throw std::logic_error("The method or operation is not implemented.");
        //FILE * f = fopen(pFileName);
        //fclose(f);
        return 0;
    }


    HRESULT Close(THIS_ LPCVOID pData)
    {
        return 0;
    }
};

DXShaderCompiler::DXShaderCompiler()
{
}


DXShaderCompiler::~DXShaderCompiler()
{
}

HRESULT DXShaderCompiler::Compile(DXShader & output, const std::wstring & path, DXShaderType shaderType, const std::string & entry)
{
	DXShader * shader = &output;

#if defined(_DEBUG)
	// Enable better shader debugging with the graphics debugging tools.
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif

	const char * profile = nullptr;
	switch (shaderType)
	{
	case DXShaderType::VS:
		profile = "vs_5_1";
		break;
	case DXShaderType::PS:
		profile = "ps_5_1";
		break;
	default:
		assert(0);
	}

    DXShaderIncludeInterface includeInterface;

	ComPtr<ID3DBlob> errorMsg;
	HRESULT hr = D3DCompileFromFile(path.c_str(), nullptr, &includeInterface, entry.c_str(), profile, compileFlags, 0, &shader->m_blob, &errorMsg);
	if (FAILED(hr))
	{
		::OutputDebugStringA((LPCSTR)errorMsg->GetBufferPointer());
		assert(0);
	}
	return hr;
}
