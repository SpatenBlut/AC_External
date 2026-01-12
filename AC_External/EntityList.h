#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include "offset.h"
#include "memory.h"

struct Enemy {
    float x;
    float y;
    float z;
};

std::vector<Enemy>GetEntitys(HANDLE hProcess);
