#include "gui.h"
#include "globals.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

// External declaration of ImGui's Win32 window procedure handler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND window,
    UINT message,
    WPARAM wideParameter,
    LPARAM longParameter
);

// Window procedure callback function
LRESULT CALLBACK WindowProcess(
    HWND window,
    UINT message,
    WPARAM wideParameter,
    LPARAM longParameter)
{
    // Handle ImGui-related window messages
    if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
        return true;

    switch (message)
    {
    case WM_SIZE: {
        // Handle window resizing
        if (gui::device && wideParameter != SIZE_MINIMIZED)
        {
            gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
            gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
            gui::ResetDevice();
        }
        return 0;
    }

    case WM_SYSCOMMAND: {
        // Prevent ALT key from opening the system menu
        if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    }

    case WM_DESTROY: {
        // Handle window destruction
        PostQuitMessage(0);
        return 0;
    }

    case WM_LBUTTONDOWN: {
        // Handle left mouse button press (used for window dragging)
        gui::position = MAKEPOINTS(longParameter); // set click points
        return 0;
    }

    case WM_MOUSEMOVE: {
        // Handle mouse movement for window dragging
        if (wideParameter == MK_LBUTTON)
        {
            const auto points = MAKEPOINTS(longParameter);
            auto rect = RECT{};

            GetWindowRect(gui::window, &rect);

            // Adjust window position based on mouse movement
            rect.left += points.x - gui::position.x;
            rect.top += points.y - gui::position.y;

            if (gui::position.x >= 0 &&
                gui::position.x <= gui::WIDTH &&
                gui::position.y >= 0 && gui::position.y <= 19)
                SetWindowPos(
                    gui::window,
                    HWND_TOPMOST,
                    rect.left,
                    rect.top,
                    0, 0,
                    SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
                );
        }
        return 0;
    }

    }

    // Default window procedure for unhandled messages
    return DefWindowProc(window, message, wideParameter, longParameter);
}

// Function to create the window
void gui::CreateHWindow(const char* windowName) noexcept
{
    // Define and register the window class
    WNDCLASSEXA windowClass = { sizeof(WNDCLASSEXA), CS_CLASSDC, WindowProcess, 0L, 0L,
                                GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
                                "class001", nullptr };
    RegisterClassExA(&windowClass);

    // Create the window
    window = CreateWindowExA(
        0,
        "class001",
        windowName,
        WS_POPUP,
        100,
        100,
        WIDTH,
        HEIGHT,
        0,
        0,
        windowClass.hInstance,
        0
    );

    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);
}

// Function to destroy the window
void gui::DestroyHWindow() noexcept
{
    DestroyWindow(window);
    UnregisterClassA("class001", GetModuleHandle(nullptr));
}

// Function to create the Direct3D device
bool gui::CreateDevice() noexcept
{
    // Create the Direct3D object
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (!d3d)
        return false;

    ZeroMemory(&presentParameters, sizeof(presentParameters));

    // Set up the presentation parameters for the Direct3D device
    presentParameters.Windowed = TRUE;
    presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    presentParameters.EnableAutoDepthStencil = TRUE;
    presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
    presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    // Create the Direct3D device
    if (d3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        window,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &presentParameters,
        &device) < 0)
        return false;

    return true;
}

// Function to reset the Direct3D device
void gui::ResetDevice() noexcept
{
    ImGui_ImplDX9_InvalidateDeviceObjects();

    const auto result = device->Reset(&presentParameters);

    if (result == D3DERR_INVALIDCALL)
        IM_ASSERT(0);

    ImGui_ImplDX9_CreateDeviceObjects();
}

// Function to destroy the Direct3D device
void gui::DestroyDevice() noexcept
{
    if (device)
    {
        device->Release();
        device = nullptr;
    }

    if (d3d)
    {
        d3d->Release();
        d3d = nullptr;
    }
}

// Function to initialize ImGui
void gui::CreateImGui() noexcept
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = NULL;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(device);
}

// Function to clean up ImGui
void gui::DestroyImGui() noexcept
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

// Function to begin rendering
void gui::BeginRender() noexcept
{
    MSG message;
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);

        if (message.message == WM_QUIT)
        {
            isRunning = !isRunning;
            return;
        }
    }

    // Start a new ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

// Function to end rendering and present the frame
void gui::EndRender() noexcept
{
    ImGui::EndFrame();

    // Set render states
    device->SetRenderState(D3DRS_ZENABLE, FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    // Clear the screen and depth buffer
    device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

    if (device->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        device->EndScene();
    }

    // Present the rendered frame
    const auto result = device->Present(0, 0, 0, 0);

    // Handle device lost situation
    if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
        ResetDevice();
}

// Function to render the ImGui interface
void gui::Render() noexcept
{
    // Set up ImGui window properties
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
    ImGui::Begin(
        "voltizap",
        &isRunning,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove
    );

    // Add ImGui slider to control FOV
    ImGui::SliderInt("Fov", &globals::FOV, 0, 200);

    ImGui::End();
}
