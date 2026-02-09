#include "offset.h"

namespace offset
{
	uintptr_t baseAddress = 0;
	uintptr_t LocalPlayer = 0;
	uintptr_t PlayerCount = 0;
	uintptr_t EntityList = 0;
	uintptr_t HealthOffset = 0;
	uintptr_t HeadPositionX = 0;
	uintptr_t HeadPositionY = 0;
	uintptr_t HeadPositionZ = 0;
	uintptr_t Yaw = 0;
	uintptr_t Pitch = 0;
	uintptr_t RifleAmmo = 0;
	uintptr_t SubmachineAmmo = 0;
	uintptr_t SniperAmmo = 0;
	uintptr_t PistolAmmo = 0;
	uintptr_t GrenadeAmmo = 0;
	uintptr_t FastFireRifle = 0;

	void Init(uintptr_t base) {
		baseAddress = base;
		LocalPlayer = baseAddress + 0x0017E0A8;
		PlayerCount = baseAddress + 0x18AC0C;
		EntityList = baseAddress + 0x18AC04;
		HealthOffset = 0xEC;
		FastFireRifle = 0x164;
		RifleAmmo = 0x140;
		SubmachineAmmo = 0x138;
		SniperAmmo = 0x13C;
		PistolAmmo = 0x12C;
		GrenadeAmmo = 0x144;
		HeadPositionX =  0x4;
		HeadPositionY =  0x8;
		HeadPositionZ =  0xC;
		Yaw = 0x34;
		Pitch = 0x38;
	}
}
