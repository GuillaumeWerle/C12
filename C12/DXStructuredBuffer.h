#pragma once
#include "DXUploadable.h"
#include "DXDescriptorHandle.h"

class DXBuffer;

class DXStructuredBuffer : public DXUploadable
{
public:
	DXDescriptorHandle m_srv;

	void Init(u32 count, u32 stride, void * data = nullptr);
	u8 * Map();

	virtual void Upload(DXRenderContext * rc) override;

	DXStructuredBuffer();
	~DXStructuredBuffer();

protected:
	DXBuffer * m_upload = nullptr;
	DXBuffer * m_commited = nullptr;
	D3D12_SHADER_RESOURCE_VIEW_DESC m_srvDesc = {};
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT m_placedFootprint = {};
	u32 m_rowCount = 0;
	u64 m_pitchInBytes = 0;
	u64 m_totalBytes = 0;
};

