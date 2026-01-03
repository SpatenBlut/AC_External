#include <iostream>
#include "memory.h"

int main()
{

	DWORD processId = mem.GetPIDByProcessName(L"ac_client.exe"); // Get Process ID
	HANDLE hProcess = mem.OpenProcessHandle(processId); // Open Process Handle
	uintptr_t baseAddress = mem.GetProcessBaseAddress(processId); // Get Base Address


	DWORD rpmAddress = 0x007ADD2C; //Address for RPM
	DWORD rpmOutput = mem.ReadMemory(hProcess, rpmAddress); // Read RPM Value
	std::cout << rpmOutput << std::endl; // Print RPM Output


	DWORD wpmAddress = 0x007ADD2C; //Address for WPM
	int WPMValue = 9000; // WPM Value
	DWORD wpmOutput = mem.WriteMemory(hProcess, wpmAddress, WPMValue); // Write WPM Value
	std::cout << wpmOutput; // Print WPM Output
	

	CloseHandle(hProcess);// close handle
	
	return 0;
}
