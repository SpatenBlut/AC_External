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
	extern uintptr_t HeadPositionX;
	extern uintptr_t HeadPositionY;
	extern uintptr_t HeadPositionZ;
	extern uintptr_t PlayerCameraX;
	extern uintptr_t PlayerCameraY;
	extern uintptr_t RifleAmmo;
	extern uintptr_t SubmachineAmmo;
	extern uintptr_t SniperAmmo;
	extern uintptr_t PistolAmmo;
	extern uintptr_t GrenadeAmmo;
	extern uintptr_t FastFireRifle;
}