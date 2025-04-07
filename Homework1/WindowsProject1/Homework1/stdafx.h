// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// STL
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <ranges>
#include <type_traits>
#include <random>

using tstring = std::basic_string<_TCHAR>;

// DirectXMath
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

using namespace DirectX;
using namespace DirectX::PackedVector;


constexpr static DWORD FRAME_BUFFER_WIDTH	= 640;
constexpr static DWORD FRAME_BUFFER_HEIGHT	= 480;

constexpr static UINT8 DIR_FORWARD		= 0x01;
constexpr static UINT8 DIR_BACKWARD		= DIR_FORWARD << 1;
constexpr static UINT8 DIR_LEFT			= DIR_BACKWARD << 1;
constexpr static UINT8 DIR_RIGHT		= DIR_LEFT << 1;
constexpr static UINT8 DIR_UP			= DIR_RIGHT << 1;
constexpr static UINT8 DIR_DOWN			= DIR_UP << 1;

constexpr static UINT8 EXPLOSION_DEBRISES = 240;

inline bool IsZero(float fValue) { return fabsf(fValue) < std::numeric_limits<float>::epsilon(); }
inline bool IsEqual(float fA, float fB) { return (::IsZero(fA - fB)); }


// Additional Headers
#include "RandomGenerator.h"
#include "XMMathHelper.h"
#include "MeshHelper.h"

#undef min
#undef max