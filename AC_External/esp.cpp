#include "esp.h"

extern HANDLE hProcess;

bool WorldToScreen(float* m, float x, float y, float z, ImVec2& out, int W, int H) {
    float clipX = m[0] * x + m[4] * y + m[8] * z + m[12];
    float clipY = m[1] * x + m[5] * y + m[9] * z + m[13];
    float clipW = m[3] * x + m[7] * y + m[11] * z + m[15];

    if (clipW < 0.1f) return false;

    float ndcX = clipX / clipW;
    float ndcY = clipY / clipW;

    out.x = (W / 2.f * ndcX) + (ndcX + W / 2.f);
    out.y = -(H / 2.f * ndcY) + (ndcY + H / 2.f);
    return true;
}

void HeadCircle(ImDrawList* dl, std::vector<Enemy>& enemies, float* matrix, int W, int H) {
    for (auto& e : enemies) {
        if (e.Health <= 0) continue;

        ImVec2 screenHead;
        if (!WorldToScreen(matrix, e.x, e.y, e.z, screenHead, W, H)) continue;

        screenHead.y += 22.5f;

        dl->AddCircle(screenHead, 10.f, IM_COL32(255, 0, 0, 255), 32, 2.f);
    }
}
void esp() {
    ImDrawList* dl = ImGui::GetBackgroundDrawList();

    HWND gameWnd = FindWindowA(nullptr, "AssaultCube");
    RECT clientRect;
    GetClientRect(gameWnd, &clientRect);
    int W = clientRect.right;
    int H = clientRect.bottom;

    float matrix[16];
    for (int i = 0; i < 16; i++)
        matrix[i] = mem.ReadMemory<float>(hProcess, offset::ViewMatrix + i * 4);

    std::vector<Enemy> enemies = GetEntitys(hProcess);

    HeadCircle(dl, enemies, matrix, W, H);
}