#pragma once

#include "TypeDefinition.h"
#include "ValueDefinition.h"
#include "StructDefinition.h"

// STL
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

// WIN
#include <windows.h>
#include <assert.h>
#define _HAS_STD_BYTE 0

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif

#include "../resource.h"

#define MAX_NAME_STRING 256
#define HInstance() GetModuleHandle(NULL)

#define CHECK(p)	assert(SUCCEEDED(p))