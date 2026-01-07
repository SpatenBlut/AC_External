#include "EntityList.h"

void entitylist(HANDLE hProcess) {

	uintptr_t rpmAddress = offset::PlayerCount;
	int PlayerCount = mem.ReadMemory(hProcess, rpmAddress);

	uintptr_t listPtr = mem.ReadMemory(hProcess, offset::EntityList);

	for (int i = 1; i < PlayerCount; i++) {
		uintptr_t entityBase = mem.ReadMemory(hProcess, listPtr + i * 0x4);
		uintptr_t healthAddress = entityBase + offset::HealthOffset;
		int healthOutput = mem.ReadMemory(hProcess, healthAddress);
	}

}