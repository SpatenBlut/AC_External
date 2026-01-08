#include "mod.h"


void mod(HANDLE hProcess) {
	uintptr_t offset = mem.ReadMemory<int>(hProcess, offset::LocalPlayer);
	uintptr_t healthAddress = offset + offset::HealthOffset;
	std::cout << "Local Player Health: " << std::dec << mem.ReadMemory<int>(hProcess, healthAddress) << std::endl;

	uintptr_t wpmAddress = healthAddress; 
	int WPMValue = 9999; 
	mem.WriteMemory<int>(hProcess, wpmAddress, WPMValue);
}