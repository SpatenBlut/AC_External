#include "EntityList.h"


float convertToFloat(int rawValue) {
	float floatValue;
	memcpy(&floatValue, &rawValue, sizeof(float));
	return floatValue;
}

void entitylist(HANDLE hProcess) {

	uintptr_t rpmAddress = offset::PlayerCount;
	int PlayerCount = mem.ReadMemory(hProcess, rpmAddress);

	uintptr_t listPtr = mem.ReadMemory(hProcess, offset::EntityList);

	for (int i = 1; i < PlayerCount; i++) {
		uintptr_t entityBase = mem.ReadMemory(hProcess, listPtr + i * 0x4);

		uintptr_t HeadAddressX = entityBase + offset::HeadPositionX;
		uintptr_t HeadAddressY = entityBase + offset::HeadPositionY;
		uintptr_t HeadAddressZ = entityBase + offset::HeadPositionZ;

		int rawX = mem.ReadMemory(hProcess, HeadAddressX);
		int rawY = mem.ReadMemory(hProcess, HeadAddressY);
		int rawZ = mem.ReadMemory(hProcess, HeadAddressZ);
		
		std::cout << "Player " << i << "      X " << convertToFloat(rawX) << "    Y " << convertToFloat(rawY) << "    Z " << convertToFloat(rawZ) << std::endl; // Head Position
	}

}