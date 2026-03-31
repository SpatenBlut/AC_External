#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include <d3d11.h>
#include <dwmapi.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwmapi.lib")

#include "esp.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

static ID3D11Device* g_Device = nullptr;
static ID3D11DeviceContext* g_Context = nullptr;
static IDXGISwapChain* g_Chain = nullptr;
static ID3D11RenderTargetView* g_RTV = nullptr;

void CreateRTV() {
    ID3D11Texture2D* bb = nullptr;
    g_Chain->GetBuffer(0, IID_PPV_ARGS(&bb));
    g_Device->CreateRenderTargetView(bb, nullptr, &g_RTV);
    bb->Release();
}
void DropRTV() { if (g_RTV) { g_RTV->Release(); g_RTV = nullptr; } }

LRESULT CALLBACK WndProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    if (ImGui_ImplWin32_WndProcHandler(hw, msg, wp, lp)) return true;
    if (msg == WM_SIZE && g_Device && wp != SIZE_MINIMIZED) {
        DropRTV();
        g_Chain->ResizeBuffers(0, LOWORD(lp), HIWORD(lp), DXGI_FORMAT_UNKNOWN, 0);
        CreateRTV();
    }
    if (msg == WM_DESTROY || (msg == WM_KEYDOWN && wp == VK_INSERT))
        PostQuitMessage(0);
    return DefWindowProc(hw, msg, wp, lp);
}

void DrawEsp() {
    WNDCLASSEX wc = { sizeof(wc), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0,
        GetModuleHandle(nullptr), nullptr, LoadCursor(nullptr, IDC_ARROW),
        (HBRUSH)GetStockObject(BLACK_BRUSH), nullptr, L"OV", nullptr };
    RegisterClassEx(&wc);

    int W = GetSystemMetrics(SM_CXSCREEN);
    int H = GetSystemMetrics(SM_CYSCREEN);

    HWND hw = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE,
        L"OV", L"", WS_POPUP, 0, 0, W, H, nullptr, nullptr, wc.hInstance, nullptr);

    SetLayeredWindowAttributes(hw, 0, 255, LWA_ALPHA);
    MARGINS m = { -1,-1,-1,-1 };
    DwmExtendFrameIntoClientArea(hw, &m);
    ShowWindow(hw, SW_SHOW);

    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2; sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; sd.OutputWindow = hw;
    sd.SampleDesc.Count = 1; sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    D3D_FEATURE_LEVEL fl;
    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION, &sd, &g_Chain, &g_Device, &fl, &g_Context);
    CreateRTV();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hw);
    ImGui_ImplDX11_Init(g_Device, g_Context);

    MSG msg = {};
    while (msg.message != WM_QUIT) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg); DispatchMessage(&msg);
            if (msg.message == WM_QUIT) goto cleanup;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        esp();

        ImGui::Render();
        const float clear[4] = { 0,0,0,0 };
        g_Context->OMSetRenderTargets(1, &g_RTV, nullptr);
        g_Context->ClearRenderTargetView(g_RTV, clear);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_Chain->Present(0, 0);
    }

cleanup:
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    DropRTV();
    if (g_Chain)   g_Chain->Release();
    if (g_Context) g_Context->Release();
    if (g_Device)  g_Device->Release();
}