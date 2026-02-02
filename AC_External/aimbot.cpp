#include "Aimbot.h"

bool AimbotCheck = false;

void Aimbot(HANDLE hProcess) {

    ViewAngles localAngles = GetLocalPlayerViewAngles(hProcess);
    Position localPos = GetLocalPlayerPosition(hProcess);

    
    std::cout << "=== LocalPlayer ===" << std::endl;
    std::cout << "Pitch: " << localAngles.pitch << "  Yaw: " << localAngles.yaw << std::endl;
    std::cout << "Position - X: " << localPos.x << " Y: " << localPos.y << " Z: " << localPos.z << std::endl;
    std::cout << std::endl;
    

    std::vector<Enemy> enemies = GetEntitys(hProcess);

    float closestDistance = FLT_MAX;
    int closestEnemyIndex = -1;
    float target_yaw;
    float target_pitch;
    uintptr_t LocalPlayer = mem.ReadMemory<uintptr_t>(hProcess, offset::LocalPlayer);

    uintptr_t PitchAddress = LocalPlayer + offset::Pitch;
    uintptr_t YawAddress = LocalPlayer + offset::Yaw;

    for (size_t i = 0; i < enemies.size(); i++) {

        // calculate distance to local player
        float dx = enemies[i].x - localPos.x;
        float dy = enemies[i].y - localPos.y;
        float dz = enemies[i].z - localPos.z;        
		std::cout << "Enemy " << i << "  X: " << enemies[i].x << " Y: " << enemies[i].y << " Z: " << enemies[i].z << std::endl; // check 1 

        float horizontal_distance = sqrt(dx * dx + dy * dy);        
        std::cout << "horizontal: " << horizontal_distance << std::endl; // check 2

        float target_yaw = atan2(dy, dx) * (180.0f / 3.14159f) + 90.0f;
        float target_pitch = atan2(dz, horizontal_distance) * (180.0f / 3.14159f);        
        std::cout << "t yaw: " << target_yaw << "   t pitch: " << target_pitch << std::endl; //check 3

        float delta_yaw = target_yaw - localAngles.yaw;
        float delta_pitch = target_pitch - localAngles.pitch;
        std::cout << "Delta: Yaw " << delta_yaw << "   Pitch " << delta_pitch << "" << std::endl; // check 4

        // check 5
		std::cout << "yaw: " << PitchAddress << "   " << localAngles.yaw + delta_yaw << std::endl;
        std::cout << "pitch: " << YawAddress << "   " << localAngles.pitch + delta_pitch << std::endl;

        if (GetAsyncKeyState(VK_RBUTTON)) {
            if (AimbotCheck == true) {
                AimbotCheck = false;
            }
            else {
                AimbotCheck = true;
            }
        }
        if (AimbotCheck == true) {
			mem.WriteMemory<float>(hProcess, PitchAddress, localAngles.yaw + delta_yaw);
			mem.WriteMemory<float>(hProcess, YawAddress, localAngles.pitch + delta_pitch);
        }
    }
}