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
        ImVec2 screenHead;
        if (!WorldToScreen(matrix, e.x, e.y, e.z, screenHead, W, H)) continue;

        screenHead.y += 22.5f;

        dl->AddCircle(
            screenHead, 
            7.0f, 
            IM_COL32(255, 255, 255, 255), 
            32, 
            2.f
        );
    }
}

void Box(ImDrawList* dl, std::vector<Enemy>& enemies, float* matrix, int W, int H) {
    for (auto& e : enemies) {
        ImVec2 screenHead, screenFeet;

        if (!WorldToScreen(matrix, e.x, e.y, e.z, screenHead, W, H)) continue;
        if (!WorldToScreen(matrix, e.x, e.y, e.z - 5.f, screenFeet, W, H)) continue;

        float boxHeight = screenFeet.y - screenHead.y;
        if (boxHeight <= 0) continue;

        float boxWidth = boxHeight * 0.4f;

        dl->AddRect(
            ImVec2(screenHead.x - boxWidth / 2.f, screenHead.y),
            ImVec2(screenHead.x + boxWidth / 2.f, screenFeet.y),
            IM_COL32(255, 255, 255, 255),
            0.f, 0, 2.f
        );
    }
}

void NameEsp(ImDrawList* dl, std::vector<Enemy>& enemies, float* matrix, int W, int H) {
    for (auto& e : enemies) {
        ImVec2 screenHead;
        if (!WorldToScreen(matrix, e.x, e.y, e.z, screenHead, W, H)) continue;

        ImVec2 textSize = ImGui::CalcTextSize(e.Name.c_str());
        dl->AddText(
            ImVec2(screenHead.x - textSize.x / 2.f, screenHead.y - textSize.y - 2.f),
            IM_COL32(255, 255, 255, 255),
            e.Name.c_str()
        );
    }
}

void HealthEsp(ImDrawList* dl, std::vector<Enemy>& enemies, float* matrix, int W, int H) {
    for (auto& e : enemies) {
        ImVec2 screenHead, screenFeet;

        if (!WorldToScreen(matrix, e.x, e.y, e.z, screenHead, W, H)) continue;
        if (!WorldToScreen(matrix, e.x, e.y, e.z - 5.f, screenFeet, W, H)) continue;

        float boxHeight = screenFeet.y - screenHead.y;
        if (boxHeight <= 0) continue;

        float boxWidth = boxHeight * 0.4f;
        float barWidth = 4.f;
        float barMargin = 3.f; // Abstand zur Box

        float leftX = screenHead.x - boxWidth / 2.f - barMargin - barWidth;
        float topY = screenHead.y;
        float botY = screenFeet.y;

        float healthPct = e.Health / 100.f;
        if (healthPct > 1.f) healthPct = 1.f;
        if (healthPct < 0.f) healthPct = 0.f;

        float filledY = botY - (boxHeight * healthPct);

        // Hintergrund (dunkelrot)
        dl->AddRectFilled(
            ImVec2(leftX, topY),
            ImVec2(leftX + barWidth, botY),
            IM_COL32(100, 0, 0, 180)
        );

        // Health Farbe: grün → gelb → rot
        ImU32 barColor;
        if (healthPct > 0.6f)
            barColor = IM_COL32(0, 255, 0, 255);
        else if (healthPct > 0.3f)
            barColor = IM_COL32(255, 200, 0, 255);
        else
            barColor = IM_COL32(255, 0, 0, 255);

        // Gefüllter Balken
        dl->AddRectFilled(
            ImVec2(leftX, filledY),
            ImVec2(leftX + barWidth, botY),
            barColor
        );

        // Rahmen
        dl->AddRect(
            ImVec2(leftX, topY),
            ImVec2(leftX + barWidth, botY),
            IM_COL32(0, 0, 0, 255),
            0.f, 0, 1.f
        );
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
    for (int i = 0; i < 16; i++) {
        matrix[i] = mem.ReadMemory<float>(hProcess, offset::ViewMatrix + i * 4);
    }

    std::vector<Enemy> enemies = GetEntitys(hProcess);

    HeadCircle(dl, enemies, matrix, W, H);
    Box(dl, enemies, matrix, W, H);
    NameEsp(dl, enemies, matrix, W, H);
    HealthEsp(dl, enemies, matrix, W, H);
}