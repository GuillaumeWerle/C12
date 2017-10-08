#include "stdafx.h"
#include "DXShaderCompiler.h"
#include "FileSystem.h"

#pragma comment (lib, "d3dcompiler.lib")

class DXShaderIncludeInterface : public ID3DInclude
{
public:

	std::vector<u8> m_data;
	
	virtual HRESULT Open(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes) override
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


    virtual HRESULT Close(THIS_ LPCVOID pData) override
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

HRESULT DXShaderCompiler::Compile(DXShader & output, const std::wstring & path, DXShaderType shaderType, const std::string & entry, const std::vector<DXMacro> & _macros)
{
	std::vector<DXMacro> macros = _macros;

	DXShader * shader = &output;

	UINT compileFlags = 0;

	const char * profile = nullptr;
	switch (shaderType)
	{
	case DXShaderType::VS:
		profile = "vs_5_1";
		macros.push_back(DXMacro("_VERTEX_SHADER"));
		break;
	case DXShaderType::PS:
		profile = "ps_5_1";
		macros.push_back(DXMacro("_PIXEL_SHADER"));
		break;
	default:
		assert(0);
	}

	std::vector<u8> shaderSource;
	FileSystem::ms_instance->ReadFile(shaderSource, FileSystem::Path(path));
	assert(shaderSource.size());	// file not found ?

	std::wstring nameUnicode(FileSystem::PathGetFilenameWithoutExtension(path));
	std::string nameASCII;
	nameASCII.assign(nameUnicode.begin(), nameUnicode.end());

	D3D_SHADER_MACRO d3dmacros[16] = {};
	assert(macros.size() < (_countof(d3dmacros) - 1));
	for (size_t i=0; i<macros.size(); i++)
	{
		d3dmacros[i].Name = macros[i].m_name.c_str();
		d3dmacros[i].Definition = macros[i].m_definition.c_str();
	}

    DXShaderIncludeInterface includeInterface;

	ComPtr<ID3DBlob> errorMsg;
	HRESULT hr = D3DCompile(&shaderSource[0], shaderSource.size(), nameASCII.c_str(), 
		d3dmacros, &includeInterface, entry.c_str(), 
		profile, compileFlags, 0, &shader->m_blob, &errorMsg);
	if (FAILED(hr))
	{
		::OutputDebugStringA((LPCSTR)errorMsg->GetBufferPointer());
		assert(0);
	}

/*
	ComPtr<ID3D12ShaderReflection> reflector;
	hr = D3DReflect(shader->m_blob->GetBufferPointer(), shader->m_blob->GetBufferSize(), IID_PPV_ARGS(&reflector));
	D3D12_SHADER_DESC shaderDesc;
	reflector->GetDesc(&shaderDesc);
	UINT idx = 0;
	D3D12_SHADER_INPUT_BIND_DESC bindDesc;
	reflector->GetResourceBindingDesc(idx, &bindDesc);
*/

	return hr;
}
