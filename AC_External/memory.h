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

    template<typename T>
    bool WriteMemory(HANDLE hProcess, uintptr_t address, T newValue) {
        BOOL result = WriteProcessMemory(hProcess, (LPVOID)address, &newValue, sizeof(T), nullptr);

        if (result) {
            return true;
        }
        else {
            std::cout << "WriteMemory Error: " << GetLastError() << std::endl;
            return false;
        }
    }

};

extern Memory mem;