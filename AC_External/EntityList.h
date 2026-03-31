#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <mutex>
#include "offset.h"
#include "memory.h"

struct Enemy {
    float x, y, z; // Kopf
    int Health;
};

extern std::vector<Enemy> g_enemies;
extern std::mutex g_enemiesMutex;

void ReadEntitys(HANDLE hProcess);                 
std::vector<Enemy> GetEntitys(HANDLE hProcess);    