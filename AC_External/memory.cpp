#include "memory.h"

Memory mem;

// Get Process ID by Process Name
DWORD Memory::GetPIDByProcessName(const wchar_t* processName) {
    HANDLE hProcessSnapshot;
    PROCESSENTRY32 PE32;

    hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
        std::cout << "CreateToolhelp32Snapshot Invalid handle";
        return 0;
    }

    PE32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnapshot, &PE32)) {
        std::cout << "Process32First Error " << GetLastError() << std::endl;
        CloseHandle(hProcessSnapshot);
        return 0;
    }

    while (Process32Next(hProcessSnapshot, &PE32)) {
        if (wcscmp(processName, PE32.szExeFile) == 0) {
            CloseHandle(hProcessSnapshot);
            return PE32.th32ProcessID;            
        }
    }

    CloseHandle(hProcessSnapshot);
	return 0;
}

// Get Process Base Address
uintptr_t Memory::GetProcessBaseAddress(DWORD processId) {
    uintptr_t baseAddress = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

    if (snapshot != INVALID_HANDLE_VALUE) {

        MODULEENTRY32W moduleEntry;
        moduleEntry.dwSize = sizeof(MODULEENTRY32W);

        if (Module32FirstW(snapshot, &moduleEntry)) {
            baseAddress = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
        }
        CloseHandle(snapshot);
    }

    if (baseAddress != 0) {
        return baseAddress; // get base address successful
    }
    else{
        std::cout << "GetProcessBaseAddress Error " << GetLastError() << std::endl; // get base address failed
	}
}

// Open Process Handle
HANDLE Memory::OpenProcessHandle(DWORD processId) {
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (processHandle != NULL) {
		return processHandle; // open process handle
    }
	else{
		std::cout << "OpenProcess Error " << GetLastError() << std::endl; // open process handle failed
	}
}
