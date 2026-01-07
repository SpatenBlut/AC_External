#include "LocalPlayer.h"

uintptr_t offsets;

void HealthHack(HANDLE hProcess) {
	offsets = mem.ReadMemory(hProcess, offset::LocalPlayer);
	uintptr_t healthAddress = offsets + offset::HealthOffset;
	DWORD wpmOutput = mem.WriteMemory(hProcess, healthAddress, 9999);
}

void AmmoHack(HANDLE hProcess) {
	uintptr_t RifleAmmoAddress = offsets + offset::RifleAmmo;
	DWORD wpmRifleOutput = mem.WriteMemory(hProcess, RifleAmmoAddress, 999);
	uintptr_t SubmachineAmmoAddress = offsets + offset::SubmachineAmmo;
	DWORD wpmSubmachineOutput = mem.WriteMemory(hProcess, SubmachineAmmoAddress, 999);
	uintptr_t SniperAmmoAddress = offsets + offset::SniperAmmo;
	DWORD wpmSniperOutput = mem.WriteMemory(hProcess, SniperAmmoAddress, 999);
	uintptr_t PistolAmmoAddress = offsets + offset::PistolAmmo;
	DWORD wpmPistolOutput = mem.WriteMemory(hProcess, PistolAmmoAddress, 999);
	uintptr_t GrenadeAmmoAddress = offsets + offset::GrenadeAmmo;
	DWORD wpmGrenadeOutput = mem.WriteMemory(hProcess, GrenadeAmmoAddress, 999);
}


void FastFire(HANDLE hProcess) {
	uintptr_t FastFireAddress = offsets + offset::FastFireRifle;
	DWORD wpmFastFireOutput = mem.WriteMemory(hProcess, FastFireAddress, 0);
}


void LocalPlayerPosition(HANDLE hProcess) {

	uintptr_t LocalPlayer = mem.ReadMemory(hProcess, offset::LocalPlayer);

	uintptr_t HeadAddressX = LocalPlayer + offset::HeadPositionX;
	uintptr_t HeadAddressY = LocalPlayer + offset::HeadPositionY;
	uintptr_t HeadAddressZ = LocalPlayer + offset::HeadPositionZ;

	int rawX = mem.ReadMemory(hProcess, HeadAddressX);
	int rawY = mem.ReadMemory(hProcess, HeadAddressY);
	int rawZ = mem.ReadMemory(hProcess, HeadAddressZ);

	std::cout << "LocalPlayer" << ", Hposition: " << "X " << convertToFloat(rawX) << "    Y " << convertToFloat(rawY) << "    Z " << convertToFloat(rawZ) << std::endl;
}

