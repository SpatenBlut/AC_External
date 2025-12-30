#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>


class Memory
{
public:
	DWORD GetPIDByProcessName(const wchar_t* processName)
	{
		DWORD PID = 0;
		HANDLE hProcessSnapshot;
		PROCESSENTRY32 PE32;

		// Take a snapshot of all processes in the system.
		hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (hProcessSnapshot == INVALID_HANDLE_VALUE)
		{
			std::cout << "CreateToolhelp32Snapshot Invalid handle";
			return 0;
		}

		// Set the size of the structure before using it.
		PE32.dwSize = sizeof(PROCESSENTRY32);

		// Retrieves information about the first process and exit if unsuccessful
		if (!Process32First(hProcessSnapshot, &PE32))
		{
			std::cout << "<Process32First> Error " << GetLastError() << '\n';
			CloseHandle(hProcessSnapshot);
			return 0;
		}

		// Now walk the snapshot of processes,
		// and find the right process then get its PID
		while (Process32Next(hProcessSnapshot, &PE32)) {

			if (wcscmp(processName, PE32.szExeFile) == 0)
			{
				PID = PE32.th32ProcessID;
				break;
			}
		}

		CloseHandle(hProcessSnapshot);
		return PID;
	}

};



int main()
{
	Memory mem;

	// Get ProcId of the target process
	std::cout << mem.GetPIDByProcessName(L"brave.exe");

	// Get module base address
	// open handle to process
	// read memory
}
