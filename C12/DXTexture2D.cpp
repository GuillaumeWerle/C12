#include "stdafx.h"
#include "DXTexture2D.h"
#include "DX.h"
#include "DXHelpers.h"
#include "DXRenderContext.h"
#include "DXBuffer.h"
#include "DirectXTex/DirectXTex/DirectXTex.h"
#include "FileSystem.h"

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
}

void DXTexture2D::Create(const D3D12_RESOURCE_DESC & desc)
{
	m_desc = desc;

	DX::Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_resource));

	m_srv.Create(m_resource.Get(), nullptr);
}

void DXTexture2D::Upload(DXRenderContext * rc)
{
	for (u32 subresourceIndex = 0; subresourceIndex < m_footPrintLayouts.size(); subresourceIndex++)
	{
		CD3DX12_TEXTURE_COPY_LOCATION dst(m_resource.Get(), subresourceIndex);
		CD3DX12_TEXTURE_COPY_LOCATION src(m_uploadBuffer->GetResource(), m_footPrintLayouts[subresourceIndex]);
		rc->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
	}
	rc->ResourceBarrier(m_resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
}


void DXTexture2D::CreateUploadBuffer()
{
	m_footPrintLayouts.resize(m_desc.MipLevels * m_desc.DepthOrArraySize);
	m_numRows.resize(m_desc.MipLevels * m_desc.DepthOrArraySize);
	m_rowSizeInBytes.resize(m_desc.MipLevels * m_desc.DepthOrArraySize);
	DX::Device->GetCopyableFootprints(&m_desc, 0, m_desc.MipLevels * m_desc.DepthOrArraySize, 0, &m_footPrintLayouts[0], &m_numRows[0], &m_rowSizeInBytes[0], &m_footPrintTotalBytes);

	m_uploadBuffer = new DXBuffer;
	m_uploadBuffer->Init(D3D12_HEAP_TYPE_UPLOAD, m_footPrintTotalBytes);
}

void DXTexture2D::LoadDDS(const FileSystem::Path & path)
{
	std::vector<u8> dds;
	FileSystem::ms_instance->ReadFile(dds, path);

	DirectX::ScratchImage scratch;
	DirectX::LoadFromDDSMemory(&dds[0], dds.size(), DirectX::DDS_FLAGS_NONE, nullptr, scratch);
	const TexMetadata& metadata = scratch.GetMetadata();

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Format = metadata.format;
	desc.Width = metadata.width;
	desc.Height = (u32)metadata.height;
	desc.DepthOrArraySize = (u16)metadata.arraySize;
	desc.MipLevels = (u16)metadata.mipLevels;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	Create(desc);

	CreateUploadBuffer();

	CopyScratchImageToUploadBuffer(scratch);

	DX::Uploader->RequestUpload(this);
}

void DXTexture2D::CreateDepthStencil(u32 width, u32 height)
{
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

	DX::Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(&m_resource));

}

void DXTexture2D::CopyScratchImageToUploadBuffer(DirectX::ScratchImage &scratch)
{
	u32 subresourceIndex = 0;
	for (u32 arrayIndex = 0; arrayIndex < m_desc.DepthOrArraySize; arrayIndex++)
	{
		for (u32 mipIndex = 0; mipIndex < m_desc.MipLevels; mipIndex++, subresourceIndex++)
		{
			const Image * img = scratch.GetImage(mipIndex, arrayIndex, 0);

			u8* destPtr = m_uploadBuffer->Map() + m_footPrintLayouts[subresourceIndex].Offset;
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
