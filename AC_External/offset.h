#pragma once
#include <Windows.h>

namespace offset
{
	extern uintptr_t baseAddress;

	void Init(uintptr_t base);

	extern uintptr_t LocalPlayer;
	extern uintptr_t PlayerCount;
	extern uintptr_t EntityList;
	extern uintptr_t HealthOffset;
	extern uintptr_t uintptr_tHeadPositionX;
	extern uintptr_t uintptr_tHeadPositionY;
	extern uintptr_t uintptr_tHeadPositionZ;
	extern uintptr_t uintptr_tPlayerCameraX;
	extern uintptr_t uintptr_tPlayerCameraY;
}