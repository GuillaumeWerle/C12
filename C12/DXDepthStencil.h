#pragma once

#include "DXSRV.h"

class DXDepthStencil
{
public:

	const DXSRV & GetSRV() const { return m_srv; }
	const DXDSV & GetDSV() const { return m_dsv; }

	void Create(u32 width, u32 height);

	DXDepthStencil();
	~DXDepthStencil();


private:
	D3D12_RESOURCE_DESC m_desc;
	ComPtr<ID3D12Resource> m_resource;
	DXSRV m_srv;
	DXDSV m_dsv;

};

