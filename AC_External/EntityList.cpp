#include "EntityList.h"


std::vector<Enemy>GetEntitys(HANDLE hProcess) {

	std::vector<Enemy> enemies;

	int PlayerCount = mem.ReadMemory<int>(hProcess, offset::PlayerCount);
	uintptr_t listPtr = mem.ReadMemory<uintptr_t>(hProcess, offset::EntityList);

	for (int i = 1; i < PlayerCount; i++) {
		uintptr_t entityBase = mem.ReadMemory<uintptr_t>(hProcess, listPtr + i * 0x4);

		uintptr_t HeadAddressX = entityBase + offset::HeadPositionX;
		uintptr_t HeadAddressY = entityBase + offset::HeadPositionY;
		uintptr_t HeadAddressZ = entityBase + offset::HeadPositionZ;

		Enemy e;
		e.x = mem.ReadMemory<float>(hProcess, entityBase + offset::HeadPositionX);
		e.y = mem.ReadMemory<float>(hProcess, entityBase + offset::HeadPositionY);
		e.z = mem.ReadMemory<float>(hProcess, entityBase + offset::HeadPositionZ);

		enemies.push_back(e); // add vector element
	}
	return enemies;
}