#include "EntityList.h"

void entitylist(HANDLE hProcess) {

	uintptr_t rpmAddress = offset::PlayerCount;
	int PlayerCount = mem.ReadMemory<int>(hProcess, rpmAddress);

	uintptr_t listPtr = mem.ReadMemory<uintptr_t>(hProcess, offset::EntityList);

	for (int i = 1; i < PlayerCount; i++) {
		uintptr_t entityBase = mem.ReadMemory<uintptr_t>(hProcess, listPtr + i * 0x4);

		uintptr_t HeadAddressX = entityBase + offset::HeadPositionX;
		uintptr_t HeadAddressY = entityBase + offset::HeadPositionY;
		uintptr_t HeadAddressZ = entityBase + offset::HeadPositionZ;

		float OutputX = mem.ReadMemory<float>(hProcess, HeadAddressX);
		float OutputY = mem.ReadMemory<float>(hProcess, HeadAddressY);
		float OutputZ = mem.ReadMemory<float>(hProcess, HeadAddressZ);
		
		std::cout << "Player " << i << "      X " << OutputX << "    Y " << OutputY << "    Z " << OutputZ << std::endl; // Head Position
	}

}