#include "LocalPlayer.h"

uintptr_t offsets;

void HealthHack(HANDLE hProcess) {
	offsets = mem.ReadMemory <uintptr_t>(hProcess, offset::LocalPlayer);
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

void LocalPlayerViewAngel(HANDLE hProcess) {
	uintptr_t LocalPlayer = mem.ReadMemory<uintptr_t>(hProcess, offset::LocalPlayer);

	uintptr_t Pitch = LocalPlayer + offset::PlayerCameraY; // y Angel
	uintptr_t Yaw = LocalPlayer + offset::PlayerCameraX; // x Angel

	float PitchOutput = mem.ReadMemory<float>(hProcess, Pitch);
	float YawOutput = mem.ReadMemory<float>(hProcess, Yaw);

	std::cout << "LocalPlayer" << "   Pitch " << PitchOutput << "    Yaw " << YawOutput << std::endl; // View Angel
}

void LocalPlayerPosition(HANDLE hProcess) {

	uintptr_t LocalPlayer = mem.ReadMemory <uintptr_t>(hProcess, offset::LocalPlayer);

	uintptr_t HeadAddressX = LocalPlayer + offset::HeadPositionX;
	uintptr_t HeadAddressY = LocalPlayer + offset::HeadPositionY;
	uintptr_t HeadAddressZ = LocalPlayer + offset::HeadPositionZ;

	float OutputX = mem.ReadMemory<float>(hProcess, HeadAddressX);
	float OutputY = mem.ReadMemory<float>(hProcess, HeadAddressY);
	float OutputZ = mem.ReadMemory<float>(hProcess, HeadAddressZ);

	std::cout << "LocalPlayer" << "   X " << OutputX << "    Y " << OutputY << "    Z " << OutputZ << std::endl; // Head Position
}

// Neue Funktion: Gibt ViewAngles als Struct zurück
ViewAngles GetLocalPlayerViewAngles(HANDLE hProcess) {
	ViewAngles angles;

	uintptr_t LocalPlayer = mem.ReadMemory<uintptr_t>(hProcess, offset::LocalPlayer);

	uintptr_t Pitch = LocalPlayer + offset::PlayerCameraY; // y Angel
	uintptr_t Yaw = LocalPlayer + offset::PlayerCameraX; // x Angel

	angles.pitch = mem.ReadMemory<float>(hProcess, Pitch);
	angles.yaw = mem.ReadMemory<float>(hProcess, Yaw);

	return angles;
}

// Neue Funktion: Gibt Position als Struct zurück
Position GetLocalPlayerPosition(HANDLE hProcess) {
	Position pos;

	uintptr_t LocalPlayer = mem.ReadMemory<uintptr_t>(hProcess, offset::LocalPlayer);

	pos.x = mem.ReadMemory<float>(hProcess, LocalPlayer + offset::HeadPositionX);
	pos.y = mem.ReadMemory<float>(hProcess, LocalPlayer + offset::HeadPositionY);
	pos.z = mem.ReadMemory<float>(hProcess, LocalPlayer + offset::HeadPositionZ);

	return pos;
}