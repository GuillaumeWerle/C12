#include "stdafx.h"
#include "DXHelpers.h"

const D3D12_HEAP_PROPERTIES* DXHelpers::GetDefaultHeapProps()
{
	static D3D12_HEAP_PROPERTIES heapProps =
	{
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0,
	};

	return &heapProps;
}

const D3D12_HEAP_PROPERTIES* DXHelpers::GetUploadHeapProps()
{
	static D3D12_HEAP_PROPERTIES heapProps =
	{
		D3D12_HEAP_TYPE_UPLOAD,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0,
	};

	return &heapProps;
}

const D3D12_HEAP_PROPERTIES* DXHelpers::GetReadbackHeapProps()
{
	static D3D12_HEAP_PROPERTIES heapProps =
	{
		D3D12_HEAP_TYPE_READBACK,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0,
	};

	return &heapProps;
}
