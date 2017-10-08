#include "stdafx.h"
#include "DXDepthStencil.h"

DXDepthStencil::DXDepthStencil()
{
}


DXDepthStencil::~DXDepthStencil()
{
}

void DXDepthStencil::Create(u32 width, u32 height)
{
	HRESULT hr = S_OK;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	desc.Width = width;
	desc.Height = height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	m_desc = desc;

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;
	clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	CHECK_D3DOK(hr, DX::Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(&m_resource)));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvdesc = {};
	srvdesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvdesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvdesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvdesc.Texture2D.MipLevels = 1;
	srvdesc.Texture2D.MostDetailedMip = 0;
	srvdesc.Texture2D.PlaneSlice = 0;
	srvdesc.Texture2D.ResourceMinLODClamp = 0.0f;
	m_srv.Create(m_resource.Get(), &srvdesc);

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvdesc = {};
	dsvdesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvdesc.Texture2D.MipSlice = 0;
	m_dsv.Create(m_resource.Get());

}
