#include <iostream>
#include "memory.h"

int main()
{

	DWORD processId = mem.GetPIDByProcessName(L"ac_client.exe"); // Get Process ID

	HANDLE hProcess = mem.OpenProcessHandle(processId); // Open Process Handle

	uintptr_t baseAddress = mem.GetProcessBaseAddress(processId); // Get Base Address

	DWORD rpmAddress = 0x006EF294; //Address for RPM
	DWORD rpmOutput = mem.ReadMemory(hProcess, rpmAddress); // Read RPM Value
	std::cout << "RPM Value: " << rpmOutput << std::endl; // Output RPM Value

	DWORD wpmAddress = 0x006EF294; //Address for WPM
	int WPMValue = 9000; // WPM Value
	mem.WriteMemory(hProcess, wpmAddress, WPMValue); // Write WPM Value
	

	CloseHandle(hProcess);// close handle
	
	return 0;
}
