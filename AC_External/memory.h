#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

class Memory {
public:
	DWORD GetPIDByProcessName(const wchar_t* processName);
	uintptr_t GetProcessBaseAddress(DWORD processId);
	HANDLE OpenProcessHandle(DWORD processId);

    template<typename T>
    T ReadMemory(HANDLE hProcess, uintptr_t address) {
        T value;
        BOOL result = ReadProcessMemory(hProcess, (LPCVOID)address, &value, sizeof(T), nullptr);

        if (!result) {
            std::cout << "ReadMemory Error: " << GetLastError() << std::endl;
            return T{};
        }
        return value;
    }

	DWORD WriteMemory(HANDLE hProcess, uintptr_t address, int newValue);
};

extern Memory mem;