#include "Aimbot.h"

void Aimbot(HANDLE hProcess) {

    std::vector<Enemy> enemies = GetEntitys(hProcess);

    for (size_t i = 0; i < enemies.size(); i++) {
        std::cout << "Enemie" << i
            << ": X=" << enemies[i].x
            << " Y=" << enemies[i].y
            << " Z=" << enemies[i].z
            << std::endl;
    }
}
