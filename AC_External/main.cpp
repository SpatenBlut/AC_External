#include <iostream>
#include "offset.h"
#include "memory.h"
#include "EntityList.h"
#include "LocalPlayer.h"

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

		LocalPlayerViewAngel(hProcess);
		LocalPlayerPosition(hProcess);
		entitylist(hProcess);

		Sleep(5);
	}	

	CloseHandle(hProcess);// close handle
	
	return 0;
}
