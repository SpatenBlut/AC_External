#include "EntityList.h"

std::vector<Enemy> g_enemies;
std::mutex g_enemiesMutex;

void ReadEntitys(HANDLE hProcess) {
    std::vector<Enemy> tmp;

    int PlayerCount = mem.ReadMemory<int>(hProcess, offset::PlayerCount);
    uintptr_t listPtr = mem.ReadMemory<uintptr_t>(hProcess, offset::EntityList);

    if (PlayerCount <= 0 || PlayerCount > 32) return; // ✅ ungültiger Count
    if (listPtr == 0) return;                          // ✅ ungültiger Pointer

    for (int i = 1; i < PlayerCount; i++) {
        uintptr_t entityBase = mem.ReadMemory<uintptr_t>(hProcess, listPtr + i * 0x4);

        if (entityBase == 0) continue; // ✅ toter/ungültiger Eintrag

        Enemy e;
        e.Health = mem.ReadMemory<int>(hProcess, entityBase + offset::HealthOffset);

        if (e.Health <= 0 || e.Health > 100) continue; // ✅ erst Health checken, dann weiterlesen

        e.x = mem.ReadMemory<float>(hProcess, entityBase + offset::HeadPositionX);
        e.y = mem.ReadMemory<float>(hProcess, entityBase + offset::HeadPositionY);
        e.z = mem.ReadMemory<float>(hProcess, entityBase + offset::HeadPositionZ);

        char nameBuf[32] = {};
        ReadProcessMemory(hProcess, (LPCVOID)(entityBase + offset::PlayerName), nameBuf, sizeof(nameBuf), nullptr);
        e.Name = std::string(nameBuf);

        tmp.push_back(e);
    }

    std::lock_guard<std::mutex> lock(g_enemiesMutex);
    g_enemies = std::move(tmp);
}

std::vector<Enemy> GetEntitys(HANDLE hProcess) {
    std::lock_guard<std::mutex> lock(g_enemiesMutex);
    return g_enemies;
}