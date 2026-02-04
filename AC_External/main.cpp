#include <iostream>
#include "offset.h"
#include "memory.h"
#include "EntityList.h"
#include "LocalPlayer.h"
#include "aimbot.h"

int main()
{
	DWORD processId = mem.GetPIDByProcessName(L"ac_client.exe"); // Get Process ID
	HANDLE hProcess = mem.OpenProcessHandle(processId); // Open Process Handle
	uintptr_t baseAddress = mem.GetProcessBaseAddress(processId); // Get Base Address

	// Initialis Offsets
	offset::Init(baseAddress);

	std::cout << "AC External Injected!\n";
	Sleep(1000);


	while (true) {

		HealthHack(hProcess);
		AmmoHack(hProcess);
		FastFire(hProcess);
		//Bhop

		system("cls");

		aimbot(hProcess);
		if (GetAsyncKeyState(VK_INSERT)) {
			Sleep(10000);
		}

		Sleep(5);
	}

	CloseHandle(hProcess);// close handle
	std::cin.get();

	return 0;
}