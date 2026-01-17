#include "Aimbot.h"


void Aimbot(HANDLE hProcess) {

    ViewAngles localAngles = GetLocalPlayerViewAngles(hProcess);
    Position localPos = GetLocalPlayerPosition(hProcess);

    std::cout << "=== LocalPlayer ===" << std::endl;
    std::cout << "Pitch: " << localAngles.pitch << "  Yaw: " << localAngles.yaw << std::endl;
    std::cout << "Position - X: " << localPos.x << " Y: " << localPos.y << " Z: " << localPos.z << std::endl;
    std::cout << std::endl;

    // Get Enemies
    std::vector<Enemy> enemies = GetEntitys(hProcess);
    std::cout << "=== Enemies ===" << std::endl;

    float closestDistance = FLT_MAX;
    int closestEnemyIndex = -1;

    for (size_t i = 0; i < enemies.size(); i++) {
		// calculate distance to local player
        float dx = enemies[i].x - localPos.x;
        float dy = enemies[i].y - localPos.y;
        float dz = enemies[i].z - localPos.z;
        float distance = sqrt(dx * dx + dy * dy + dz * dz);

		// print enemy info
        std::cout << "Enemy " << i
            << ": X=" << enemies[i].x
            << " Y=" << enemies[i].y
            << " Z=" << enemies[i].z
            << " | Distance: " << distance
            << std::endl;

		// check for closest enemy
        if (distance < closestDistance) {
            closestDistance = distance;
            closestEnemyIndex = i;
        }
    }

	// print closest enemy info
    if (closestEnemyIndex != -1) {
        std::cout << "\n=== Closest Enemy ===" << std::endl;
        std::cout << "Index: " << closestEnemyIndex << std::endl;
        std::cout << "Distance: " << closestDistance << std::endl;
        std::cout << "Position: X=" << enemies[closestEnemyIndex].x
            << " Y=" << enemies[closestEnemyIndex].y
            << " Z=" << enemies[closestEnemyIndex].z << std::endl;
    }
}