// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>

#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"
#include <wrl.h>
#include <shellapi.h>

using Microsoft::WRL::ComPtr;

// TODO: reference additional headers your program requires here
#include <stdint.h>
#include <vector>
#include <string>
#include <array>
#include <memory>

#include "Types.h"

#ifdef _DEBUG
	#define CHECK_D3DOK(_hr, _code) { _hr = _code; assert(SUCCEEDED(_hr)); }
#else
	#define CHECK_D3DOK(_hr, _code) { _hr = _code; }
#endif