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
	uintptr_t PlayerCameraX = 0;
	uintptr_t PlayerCameraY = 0;

	void Init(uintptr_t base) {
		baseAddress = base;
		LocalPlayer = baseAddress + 0x0017E0A8;
		PlayerCount = baseAddress + 0x18AC0C;
		EntityList = baseAddress + 0x18AC04;

		HealthOffset = 0xEC;
		HeadPositionX =  0x4;
		HeadPositionY =  0xC;
		HeadPositionZ =  0x8;
		PlayerCameraX =  0x34C;
		PlayerCameraY =  0x38;
	}
}