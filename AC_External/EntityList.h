#pragma once
#include <windows.h>
#include <vector>
#include <mutex>

struct Enemy {
    float x, y, z;
    int Health;
};

extern std::vector<Enemy> g_Enemies;
extern std::mutex          g_EnemiesMutex;  // immer locken bevor du g_Enemies liest/schreibst

void UpdateEntityList(HANDLE hProcess);
std::vector<Enemy> GetEntitys(HANDLE hProcess);