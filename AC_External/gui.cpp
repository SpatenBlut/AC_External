#include "gui.h"
#include <thread>
#include <d3d11.h>
#include <dwmapi.h>
#include <timeapi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "winmm.lib")

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "menu.h"

// --- Shared state (read/written across threads) ---
std::atomic<float>     g_fps{ 0.0f };
std::atomic<bool>      enableHealthHack{ false };
std::atomic<bool>      enableAmmoHack{ false };
std::atomic<bool>      enableFastFire{ false };
std::atomic<bool>      enableAimbot{ false };
std::atomic<DWORD>     g_pid{ 0 };
std::atomic<uintptr_t> g_baseAddress{ 0 };
HANDLE                 g_hProcess = NULL;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

// --- DX11 objects ---
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

static bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // FLIP_DISCARD breaks WS_EX_LAYERED

    const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    D3D_FEATURE_LEVEL featureLevel;

    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        featureLevels, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);

    // Fallback to software renderer
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0,
            featureLevels, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);

    if (res != S_OK) return false;

    // Create render target from back buffer
    ID3D11Texture2D* pBackBuffer = nullptr;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer) { g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView); pBackBuffer->Release(); }

    return true;
}

static void CleanupDeviceD3D()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
    if (g_pSwapChain) { g_pSwapChain->Release();           g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release();    g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release();           g_pd3dDevice = nullptr; }
}

// Window message handler
static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;
    if (msg == WM_SIZE && g_pd3dDevice && wParam != SIZE_MINIMIZED)
    {
        // Rebuild render target on resize
        if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
        g_pSwapChain->ResizeBuffers(0, LOWORD(lParam), HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
        ID3D11Texture2D* pBackBuffer = nullptr;
        g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        if (pBackBuffer) { g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView); pBackBuffer->Release(); }
        return 0;
    }
    if (msg == WM_DESTROY) { ::PostQuitMessage(0); return 0; }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

// ImGui dark theme
static void ApplyStyle()
{
    ImGuiStyle& s = ImGui::GetStyle();
    s.WindowRounding = 6.f; s.FrameRounding = 4.f; s.GrabRounding = 4.f;
    s.WindowTitleAlign = { 0.5f, 0.5f }; s.WindowPadding = { 12, 12 }; s.ItemSpacing = { 8, 6 };

    ImVec4* c = s.Colors;
    c[ImGuiCol_WindowBg] = { 0.10f, 0.10f, 0.13f, 0.95f };
    c[ImGuiCol_TitleBg] = { 0.08f, 0.08f, 0.10f, 1.00f };
    c[ImGuiCol_TitleBgActive] = { 0.14f, 0.14f, 0.20f, 1.00f };
    c[ImGuiCol_FrameBg] = { 0.18f, 0.18f, 0.22f, 1.00f };
    c[ImGuiCol_FrameBgHovered] = { 0.22f, 0.22f, 0.28f, 1.00f };
    c[ImGuiCol_FrameBgActive] = { 0.25f, 0.25f, 0.32f, 1.00f };
    c[ImGuiCol_CheckMark] = { 0.26f, 0.59f, 0.98f, 1.00f };
    c[ImGuiCol_Button] = { 0.20f, 0.40f, 0.70f, 0.80f };
    c[ImGuiCol_ButtonHovered] = { 0.26f, 0.59f, 0.98f, 1.00f };
    c[ImGuiCol_ButtonActive] = { 0.06f, 0.53f, 0.98f, 1.00f };
    c[ImGuiCol_Separator] = { 0.28f, 0.28f, 0.35f, 1.00f };
    c[ImGuiCol_Text] = { 0.90f, 0.90f, 0.95f, 1.00f };
    c[ImGuiCol_TextDisabled] = { 0.50f, 0.50f, 0.55f, 1.00f };
    c[ImGuiCol_Border] = { 0.30f, 0.30f, 0.40f, 0.60f };
}

// Main overlay thread — window creation, DX11, render loop
static void GuiThread()
{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    timeBeginPeriod(1);       // 1ms Windows timer resolution
    DwmEnableMMCSS(TRUE);     // prevent DWM throttling

    const int scrW = GetSystemMetrics(SM_CXSCREEN);
    const int scrH = GetSystemMetrics(SM_CYSCREEN);

    // Register and create fullscreen transparent topmost window
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0, 0, GetModuleHandleW(nullptr), nullptr, nullptr, nullptr, nullptr, L"AC_Overlay" };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowExW(
        WS_EX_APPWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        wc.lpszClassName, L"AC External", WS_POPUP,
        0, 0, scrW, scrH, nullptr, nullptr, wc.hInstance, nullptr);

    ::SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);           // required for DX11 alpha
    MARGINS margins = { -1, -1, -1, -1 };
DwmExtendFrameIntoClientArea(hwnd, &margins);  // per-pixel alpha via DWM

    if (!CreateDeviceD3D(hwnd)) { CleanupDeviceD3D(); ::UnregisterClassW(wc.lpszClassName, wc.hInstance); timeEndPeriod(1); return; }

    ::ShowWindow(hwnd, SW_SHOW);
    ::UpdateWindow(hwnd);

    // ImGui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
    ApplyStyle();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // INSERT toggles menu — fires via WM_HOTKEY even when game has focus
    ::RegisterHotKey(hwnd, 1, 0, VK_INSERT);
    bool menuVisible = false;

    // Toggle click-through and focus between overlay and game
    auto toggleMenu = [&](bool open)
        {
            menuVisible = open;
            LONG ex = ::GetWindowLongW(hwnd, GWL_EXSTYLE);
            if (open)
            {
                ::SetWindowLongW(hwnd, GWL_EXSTYLE, ex & ~WS_EX_TRANSPARENT);
                DWORD t1 = ::GetCurrentThreadId(), t2 = ::GetWindowThreadProcessId(::GetForegroundWindow(), nullptr);
                ::AttachThreadInput(t2, t1, TRUE); ::BringWindowToTop(hwnd); ::SetForegroundWindow(hwnd); ::AttachThreadInput(t2, t1, FALSE);
            }
            else
            {
                ::SetWindowLongW(hwnd, GWL_EXSTYLE, ex | WS_EX_TRANSPARENT);
                HWND hGame = ::FindWindowA("SDL_app", nullptr);
                if (!hGame) hGame = ::GetNextWindow(hwnd, GW_HWNDNEXT);
                if (hGame) { DWORD t1 = ::GetCurrentThreadId(), t2 = ::GetWindowThreadProcessId(hwnd, nullptr); ::AttachThreadInput(t2, t1, TRUE); ::SetForegroundWindow(hGame); ::AttachThreadInput(t2, t1, FALSE); }
            }
        };

    // --- Render loop ---
    bool done = false;
    LARGE_INTEGER freq, last, curr;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&last);
    float fpsTimer = 0.0f;

    while (!done)
    {
        // Process Windows messages
        MSG msg;
        while (::PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg); ::DispatchMessageW(&msg);
            if (msg.message == WM_HOTKEY && msg.wParam == 1) toggleMenu(!menuVisible);
            if (msg.message == WM_QUIT) done = true;
        }
        if (done) break;

        // Build and render ImGui frame
        ImGui_ImplDX11_NewFrame(); ImGui_ImplWin32_NewFrame(); ImGui::NewFrame();
        RenderHUD();
        if (menuVisible) RenderMenu();
        ImGui::Render();

        const float clear[4] = {};
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_pSwapChain->Present(0, 0); // VSync off

        // Cap at ~1000 FPS — reduces CPU/GPU usage significantly
        Sleep(1);

        // Update smoothed FPS counter 10x per second
        QueryPerformanceCounter(&curr);
        float delta = (float)(curr.QuadPart - last.QuadPart) / (float)freq.QuadPart;
        last = curr;
        fpsTimer += delta;
        if (fpsTimer >= 0.1f)
        {
            if (delta > 0.0f) g_fps.store(g_fps.load() * 0.9f + (1.0f / delta) * 0.1f); // smoothed
            fpsTimer = 0.0f;
        }
    }

    // Cleanup
    ::UnregisterHotKey(hwnd, 1);
    ImGui_ImplDX11_Shutdown(); ImGui_ImplWin32_Shutdown(); ImGui::DestroyContext();
    CleanupDeviceD3D();
    ::DestroyWindow(hwnd); ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    timeEndPeriod(1);
}

// Launch overlay on a detached thread
void gui()
{
    std::thread(GuiThread).detach();
}