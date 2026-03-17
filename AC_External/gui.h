#pragma once

#include <atomic>
#include <windows.h>

// ---------------------------------------------------------------
// Atomic feature flags — defined in gui.cpp, read everywhere else
// ---------------------------------------------------------------
extern std::atomic<bool> enableHealthHack;
extern std::atomic<bool> enableAmmoHack;
extern std::atomic<bool> enableFastFire;
extern std::atomic<bool> enableAimbot;

// Status info written from main() after successful init
extern std::atomic<DWORD>       g_pid;
extern std::atomic<uintptr_t>   g_baseAddress;
extern HANDLE                   g_hProcess;   // written once before gui() is called

// ---------------------------------------------------------------
// Entry point — launches the GUI window on its own thread.
// Call once after the init loop has succeeded.
// ---------------------------------------------------------------
void gui();