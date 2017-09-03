#pragma once

namespace DXHelpers
{
	const D3D12_HEAP_PROPERTIES* GetDefaultHeapProps();
	const D3D12_HEAP_PROPERTIES* GetUploadHeapProps();
	const D3D12_HEAP_PROPERTIES* GetReadbackHeapProps();
}

