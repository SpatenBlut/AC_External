#include "Aimbot.h"

bool AimbotCheck = false;
float delta_yaw;
float delta_pitch;

void aimbot(HANDLE hProcess) {

    ViewAngles localAngles = GetLocalPlayerViewAngles(hProcess);
    Position localPos = GetLocalPlayerPosition(hProcess);

    std::vector<Enemy> enemies = GetEntitys(hProcess);
    uintptr_t LocalPlayer = mem.ReadMemory<uintptr_t>(hProcess, offset::LocalPlayer);

    uintptr_t PitchAddress = LocalPlayer + offset::Pitch;
    uintptr_t YawAddress = LocalPlayer + offset::Yaw;

    for (size_t i = 0; i < enemies.size(); i++) {

        // calculate distance to local player
        float dx = enemies[i].x - localPos.x;
        float dy = enemies[i].y - localPos.y;
        float dz = enemies[i].z - localPos.z;

        float horizontal_distance = sqrt(dx * dx + dy * dy);

        float target_yaw = atan2(dy, dx) * (180.0f / 3.14159f) + 90.0f;
        float target_pitch = atan2(dz, horizontal_distance) * (180.0f / 3.14159f);

        delta_yaw = target_yaw - localAngles.yaw;
        delta_pitch = target_pitch - localAngles.pitch;
    }

    mem.WriteMemory<float>(hProcess, PitchAddress, localAngles.yaw + delta_yaw);
    mem.WriteMemory<float>(hProcess, YawAddress, localAngles.pitch + delta_pitch);
}