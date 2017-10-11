#pragma once
#include "DXUploadable.h"
#include "DXDescriptorHandle.h"
#include "DXSRV.h"

class DXBufferHeap;

class DXStructuredBuffer : public DXUploadable
{
public:
	const DXSRV & GetSRV() const { return m_srv; }
	void SetSRV(const DXSRV & val) { m_srv = val; }

	void Init(u32 count, u32 stride, void * data);


	DXStructuredBuffer();
	~DXStructuredBuffer();

	virtual void Upload(DXRenderContext * rc) override;

private:
	DXBufferHeap* m_upload = nullptr;
	DXBufferHeap* m_commited = nullptr;
	DXSRV m_srv;
	D3D12_SHADER_RESOURCE_VIEW_DESC m_srvDesc = {};
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT m_placedFootprint = {};
	u32 m_rowCount = 0;
	u64 m_pitchInBytes = 0;
	u64 m_totalBytes = 0;

	u8* Map();
	void Unmap();
};

