#include "EntityList.h"

std::vector<Enemy> g_enemies;
std::mutex g_enemiesMutex;

void ReadEntitys(HANDLE hProcess) {
    std::vector<Enemy> tmp;  // in tmp lesen

    int PlayerCount = mem.ReadMemory<int>(hProcess, offset::PlayerCount);
    uintptr_t listPtr = mem.ReadMemory<uintptr_t>(hProcess, offset::EntityList);

    for (int i = 1; i < PlayerCount; i++) {
        uintptr_t entityBase = mem.ReadMemory<uintptr_t>(hProcess, listPtr + i * 0x4);

        Enemy e;
        e.x = mem.ReadMemory<float>(hProcess, entityBase + offset::HeadPositionX);
        e.y = mem.ReadMemory<float>(hProcess, entityBase + offset::HeadPositionY);
        e.z = mem.ReadMemory<float>(hProcess, entityBase + offset::HeadPositionZ);
        e.Health = mem.ReadMemory<int>(hProcess, entityBase + offset::HealthOffset);

        tmp.push_back(e);  // in tmp, nicht g_enemies
    }

    // erst NACH der Loop atomar tauschen
    std::lock_guard<std::mutex> lock(g_enemiesMutex);
    g_enemies = std::move(tmp);
}

std::vector<Enemy> GetEntitys(HANDLE hProcess) {
    std::lock_guard<std::mutex> lock(g_enemiesMutex);
    return g_enemies;
}