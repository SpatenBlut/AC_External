#include <iostream>
#include "offset.h"
#include "memory.h"
#include "EntityList.h"

int main()
{
	DWORD processId = mem.GetPIDByProcessName(L"ac_client.exe"); // Get Process ID
	HANDLE hProcess = mem.OpenProcessHandle(processId); // Open Process Handle
	uintptr_t baseAddress = mem.GetProcessBaseAddress(processId); // Get Base Address

	// Initialisiere die Offsets mit der Base-Adresse
	offset::Init(baseAddress);


	
	uintptr_t offset = mem.ReadMemory(hProcess, offset::LocalPlayer);
	uintptr_t healthAddress = offset + offset::HealthOffset;
	std::cout << "Local Player Health: " << std::dec  << mem.ReadMemory(hProcess, healthAddress) << std::endl;


	entitylist(hProcess);
	

	


	DWORD wpmAddress = 0x008CED04; //Address for WPM
	int WPMValue = 9999; // WPM Value
	DWORD wpmOutput = mem.WriteMemory(hProcess, wpmAddress, WPMValue); // Write WPM Value
	

	CloseHandle(hProcess);// close handle
	
	return 0;
}
