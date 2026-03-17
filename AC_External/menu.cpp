#include "menu.h"
#include <windows.h>
#include <atomic>
#include "imgui/imgui.h"

// Shared FPS value updated by gui.cpp
extern std::atomic<float> g_fps;

// Always visible overlay elements (FPS, ESP info, etc.)
void RenderHUD()
{
    const float scrW = (float)GetSystemMetrics(SM_CXSCREEN);

    // Anchor window to top-right corner, no background or border
    ImGui::SetNextWindowPos(ImVec2(scrW - 5.0f, 5.0f), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(80.0f, 25.0f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("##hud", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoBringToFrontOnFocus);

    // FPS counter — pink
    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.7f, 1.0f), "FPS: %.0f", g_fps.load());

    // Add more HUD elements here

    ImGui::End();
    ImGui::PopStyleVar(2);
}

// Only visible when menu is open (INSERT to toggle)
void RenderMenu()
{
    const float scrW = (float)GetSystemMetrics(SM_CXSCREEN);
    const float scrH = (float)GetSystemMetrics(SM_CYSCREEN);

    // Center window on screen, fixed size
    ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2((scrW - 600) * 0.5f, (scrH - 500) * 0.5f), ImGuiCond_Once);
    ImGui::Begin("AC External", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // Add menu elements here

    ImGui::End();
}