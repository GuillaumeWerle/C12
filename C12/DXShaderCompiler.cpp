#include "stdafx.h"
#include "DXShaderCompiler.h"
#include "FileSystem.h"

#pragma comment (lib, "d3dcompiler.lib")

class DXShaderIncludeInterface : public ID3DInclude
{
public:

	std::vector<u8> m_data;
	
	HRESULT Open(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
    {
		bool success = FileSystem::ms_instance->ReadFile(m_data, FileSystem::Path(pFileName));
		if(success)
		{
			*ppData = &m_data[0];
			*pBytes = (UINT)m_data.size();
			return S_OK;
		}
		else
		{
			return S_FALSE;
		}
    }


    HRESULT Close(THIS_ LPCVOID pData)
    {
        return S_OK;
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
