#pragma once
#include "memory.h"
#include "offset.h"
#include <Windows.h>

// Struct für View Angles
struct ViewAngles {
    float pitch;  // x Angel
    float yaw;    // y Angel
};

// Struct für Position
struct Position {
    float x;
    float y;
    float z;
};

void HealthHack(HANDLE hProcess);
void AmmoHack(HANDLE hProcess);
void FastFire(HANDLE hProcess);
void LocalPlayerViewAngel(HANDLE hProcess);
void LocalPlayerPosition(HANDLE hProcess);

// Neue Funktionen die Werte zurückgeben
ViewAngles GetLocalPlayerViewAngles(HANDLE hProcess);
Position GetLocalPlayerPosition(HANDLE hProcess);