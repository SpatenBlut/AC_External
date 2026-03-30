#include "EntityList.h"
#include "offset.h"
#include "memory.h"

std::vector<Enemy> g_Enemies;
std::mutex          g_EnemiesMutex;

void UpdateEntityList(HANDLE hProcess) {
    int PlayerCount = mem.ReadMemory<int>(hProcess, offset::PlayerCount);
    uintptr_t listPtr = mem.ReadMemory<uintptr_t>(hProcess, offset::EntityList);

    // read komplett in lokalen buffer – mutex so kurz wie möglich halten
    std::vector<Enemy> tmp;
    tmp.reserve(PlayerCount);

    for (int i = 1; i < PlayerCount; i++) {
        uintptr_t entityBase = mem.ReadMemory<uintptr_t>(hProcess, listPtr + i * 0x4);
        if (!entityBase) continue;  // null-pointer überspringen

        Enemy e;
        e.x = mem.ReadMemory<float>(hProcess, entityBase + offset::HeadPositionX);
        e.y = mem.ReadMemory<float>(hProcess, entityBase + offset::HeadPositionY);
        e.z = mem.ReadMemory<float>(hProcess, entityBase + offset::HeadPositionZ);
        e.Health = mem.ReadMemory<int>(hProcess, entityBase + offset::HealthOffset);
        tmp.push_back(e);
    }

    // einmal swappen unter lock – minimale blockzeit
    std::lock_guard<std::mutex> lock(g_EnemiesMutex);
    g_Enemies = std::move(tmp);
}

std::vector<Enemy> GetEntitys(HANDLE hProcess) {
    UpdateEntityList(hProcess);
    return g_Enemies;
}