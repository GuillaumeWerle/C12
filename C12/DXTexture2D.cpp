#include "stdafx.h"
#include "DXTexture2D.h"
#include "DX.h"
#include "DXHelpers.h"
#include "DXRenderContext.h"
#include "DXBuffer.h"
#include "DirectXTex/DirectXTex/DirectXTex.h"
#include <limits>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex\\Bin\\Desktop_2015\\x64\\Debug\\DirectXTex.lib")
#endif

#ifdef NDEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex\\Bin\\Desktop_2015\\x64\\Release\\DirectXTex.lib")
#endif

DXTexture2D::DXTexture2D()
{

}

DXTexture2D::~DXTexture2D()
{
	delete m_uploadBuffer;
	DX::PoolSRVCBVUAV->Free(m_srv);
}

void DXTexture2D::Init()
{
	DirectX::ScratchImage scratch;
	DirectX::LoadFromDDSFile( L"data\\diffuse.dds", DirectX::DDS_FLAGS_NONE, nullptr, scratch);

	const TexMetadata& metadata = scratch.GetMetadata();

	D3D12_RESOURCE_DESC rdesc = {};
	rdesc.MipLevels = (u16)metadata.mipLevels;
	rdesc.Format = metadata.format;
	rdesc.Width = metadata.width;
	rdesc.Height = (u32)metadata.height;
	rdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	rdesc.DepthOrArraySize = (u16)metadata.arraySize;
	rdesc.SampleDesc.Count = 1;
	rdesc.SampleDesc.Quality = 0;
	rdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	rdesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	m_desc = rdesc;

	DX::Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&rdesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_resource));

	m_srv = DX::PoolSRVCBVUAV->Alloc();
	DX::Device->CreateShaderResourceView(m_resource.Get(), nullptr, m_srv.CPU);

	m_footPrintLayouts.resize(m_desc.MipLevels * m_desc.DepthOrArraySize);
	m_numRows.resize(m_desc.MipLevels * m_desc.DepthOrArraySize);
	m_rowSizeInBytes.resize(m_desc.MipLevels * m_desc.DepthOrArraySize);
	DX::Device->GetCopyableFootprints(&rdesc, 0, m_desc.MipLevels * m_desc.DepthOrArraySize, 0, &m_footPrintLayouts[0], &m_numRows[0], &m_rowSizeInBytes[0], &m_footPrintTotalBytes);

	m_uploadBuffer = new DXBuffer;
	m_uploadBuffer->Init(D3D12_HEAP_TYPE_UPLOAD, m_footPrintTotalBytes);
	//memset(m_uploadBuffer->m_cpuPtr, 0x01f, m_footPrintTotalBytes);

	u32 subresourceIndex = 0;
	for (u32 arrayIndex = 0; arrayIndex < m_desc.DepthOrArraySize; arrayIndex++)
	{
		for (u32 mipIndex = 0; mipIndex < m_desc.MipLevels; mipIndex++, subresourceIndex++)
		{
			const Image * img = scratch.GetImage(mipIndex, arrayIndex, 0);

			u8* destPtr = m_uploadBuffer->m_cpuPtr + m_footPrintLayouts[subresourceIndex].Offset;
			u8* srcPtr = img->pixels;

			u64 destPitch = m_footPrintLayouts[subresourceIndex].Footprint.RowPitch;
			u64 srcPitch = img->rowPitch;
			
			 for (u32 rowIndex = 0; rowIndex < m_numRows[subresourceIndex]; rowIndex++)
			 {
				 memcpy(destPtr, srcPtr, Min(destPitch, srcPitch));
				 destPtr += destPitch;
				 srcPtr += srcPitch;
			 }
		}
	}
}

void DXTexture2D::Upload(DXRenderContext * rc)
{
	for (u32 subresourceIndex = 0; subresourceIndex < m_footPrintLayouts.size(); subresourceIndex++)
	{
		CD3DX12_TEXTURE_COPY_LOCATION dst(m_resource.Get(), subresourceIndex);
		CD3DX12_TEXTURE_COPY_LOCATION src(m_uploadBuffer->m_buffer.Get(), m_footPrintLayouts[subresourceIndex]);
		rc->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
	}
	rc->ResourceBarrier(m_resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
}

