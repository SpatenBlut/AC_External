#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

class Memory
{
public:
	DWORD GetPIDByProcessName(const wchar_t* processName);
	uintptr_t GetProcessBaseAddress(DWORD processId);
	HANDLE OpenProcessHandle(DWORD processId);
	DWORD ReadMemory(HANDLE hProcess, DWORD address);
	DWORD WriteMemory(HANDLE hProcess, DWORD address, int newValue);
};

extern Memory mem;