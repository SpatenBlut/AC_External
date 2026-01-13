#include "Aimbot.h"

void Aimbot(HANDLE hProcess) {

    // Hole die ViewAngles vom LocalPlayer
    ViewAngles localAngles = GetLocalPlayerViewAngles(hProcess);

    // Hole die Position vom LocalPlayer
    Position localPos = GetLocalPlayerPosition(hProcess);

    // Ausgabe LocalPlayer Info
    std::cout << "=== LocalPlayer ===" << std::endl;
    std::cout << "Pitch: " << localAngles.pitch << "  Yaw: " << localAngles.yaw << std::endl;
    std::cout << "Position - X: " << localPos.x << " Y: " << localPos.y << " Z: " << localPos.z << std::endl;
    std::cout << std::endl;

    // Hole alle Enemies
    std::vector<Enemy> enemies = GetEntitys(hProcess);

    std::cout << "=== Enemies ===" << std::endl;
    for (size_t i = 0; i < enemies.size(); i++) {
        std::cout << "Enemy " << i
            << ": X=" << enemies[i].x
            << " Y=" << enemies[i].y
            << " Z=" << enemies[i].z
            << std::endl;
    }

}