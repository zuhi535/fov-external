#include "gui.h"      // Include header for GUI functions and definitions
#include "mem.h"      // Include header for memory management
#include "globals.h"  // Include header for global variables and definitions
#include "hacks.h"    // Include header for hack-related functions
#include <thread>     // Include header for multi-threading support

// Entry point for the Windows application
int __stdcall wWinMain(
    HINSTANCE instance,          // Handle to the current instance of the application
    HINSTANCE previousInstance,  // Handle to the previous instance of the application (always NULL for single-instance applications)
    PWSTR arguments,             // Command-line arguments passed to the application
    int commandShow)             // Specifies how the window is to be shown
{
    // Create a Memory object to manage memory operations for the process "cs2.exe"
    const auto mem = Memory("cs2.exe");

    // Get the base address of the "client.dll" module within the "cs2.exe" process
    globals::client = mem.GetModuleAddress("client.dll");

    // Start separate threads for different types of hacks
    std::thread(hacks::MiscThread, mem).detach();  // Thread for miscellaneous hacks
    std::thread(hacks::VisualThread, mem).detach(); // Thread for visual-related hacks
    std::thread(hacks::AimThread, mem).detach();    // Thread for aiming-related hacks

    // Initialize the GUI
    gui::CreateHWindow("Cheat Menu"); // Create the window for the GUI
    gui::CreateDevice();             // Create the Direct3D device for rendering
    gui::CreateImGui();             // Initialize ImGui for GUI rendering

    // Main loop for rendering the GUI
    while (gui::isRunning)
    {
        gui::BeginRender();   // Start the rendering frame
        gui::Render();       // Render GUI elements
        gui::EndRender();    // End the rendering frame

        std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Sleep for 5 milliseconds to reduce CPU usage
    }

    // Clean up resources
    gui::DestroyImGui(); // Shutdown ImGui and release resources
    gui::DestroyDevice(); // Release the Direct3D device
    gui::DestroyHWindow(); // Destroy the GUI window

    return EXIT_SUCCESS; // Return success status
}
