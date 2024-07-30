#pragma once
#include <cstddef>
#include <locale>
namespace offsets
{
	//offsets.hpp
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1824A18;
	constexpr std::ptrdiff_t dwEntityList = 0x19BEED0;

	constexpr std::ptrdiff_t m_vecOrigin = 0x88;
	constexpr std::ptrdiff_t m_iTeamNum = 0x3C3;
	constexpr std::ptrdiff_t m_bDormant = 0xEF;

	// client.dll
	constexpr std::ptrdiff_t m_pCameraServices = 0x1130; // CPlayer_CameraServices*
	constexpr std::ptrdiff_t m_iFOV = 0x210; // uint32
	constexpr std::ptrdiff_t m_bIsScoped = 0x22A0; // bool
}

namespace globals
{
	inline int FOV = 90;
	inline std::uintptr_t client = 0;
}
