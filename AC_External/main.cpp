#include <iostream>
#include <thread>
#include <atomic>
#include "offset.h"
#include "memory.h"
#include "EntityList.h"
#include "LocalPlayer.h"
#include "aimbot.h"
#include "dx11.h"

DWORD processId = 0;
HANDLE hProcess = NULL;
uintptr_t baseAddress = 0;
LARGE_INTEGER freq;

void Init() {
    while (true) {
        bool success = true;

        processId = mem.GetPIDByProcessName(L"ac_client.exe");
        if (processId == 0) {
            std::cout << "[Error] Process not found. Is ac_client.exe running?" << std::endl;
            success = false;
        }

        if (success) {
            hProcess = mem.OpenProcessHandle(processId);
            if (hProcess == NULL) {
                std::cout << "[Error] OpenProcessHandle failed." << std::endl;
                success = false;
            }
        }

        if (success) {
            baseAddress = mem.GetProcessBaseAddress(processId);
            if (baseAddress == 0) {
                std::cout << "[Error] GetProcessBaseAddress failed." << std::endl;
                success = false;
            }
        }

        if (success) {
            system("cls");
            std::cout << "Init successful!" << std::endl;
            std::cout << "PID:         " << processId << std::endl;
            std::cout << "Handle:      " << hProcess << std::endl;
            std::cout << "BaseAddress: 0x" << std::hex << baseAddress << std::dec << std::endl;
            break;
        }

        if (hProcess != NULL) {
            CloseHandle(hProcess);
            hProcess = NULL;
        }
        processId = 0;
        baseAddress = 0;

        std::cout << "[Retry] Waiting 10 seconds..." << std::endl;
        Sleep(10000);
    }
}

int main()
{
    Init();

    offset::Init(baseAddress);
    std::cout << "AC External Injected!\n";
    Sleep(1000);

    std::thread t1([]() {
        while (true) {
            HealthHack(hProcess);
            AmmoHack(hProcess);
            FastFire(hProcess);
            Sleep(1);
        }
    });

    std::thread t2([]() {
        while (true) {
            aimbot(hProcess);
        }
    });

	std::thread t3([]() {
        while (true) {
            ReadEntitys(hProcess);
			Sleep(1); //cpu entlasten
        }
    });

    std::thread t4([]() {
         DrawEsp();
    });

    while (true) {

	}

    t1.detach();
    t2.detach();
	t3.detach();
	t4.detach();

    CloseHandle(hProcess);
    return 0;
}