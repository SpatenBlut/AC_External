#include "esp.h"





void esp() {
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    dl->AddRect(
        { 100, 100 },           // oben-links
        { 400, 300 },           // unten-rechts
        IM_COL32(255, 0, 0, 255), // Farbe (R,G,B,A)
        0.f,                    // Rundung
        0,                      // Flags
        2.f                     // Liniendicke
    );
    dl->AddText(
        { 100, 80 },              // Position (x, y)
        IM_COL32(255, 255, 255, 255), // Farbe
        "Hallo Welt!"             // Text
    );
};