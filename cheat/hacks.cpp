#include "hacks.h"
#include "gui.h"
#include "globals.h"
#include <thread>

namespace hacks {

    // Function to handle miscellaneous hacks in a separate thread
    void MiscThread(const Memory& mem) noexcept
    {
        while (gui::isRunning) // Keep running while GUI is active
        {
            std::uint16_t desiredFov = (uint16_t)globals::FOV; // Read the desired field of view from global settings
            std::uintptr_t localPlayer = mem.Read<std::uintptr_t>(globals::client + offsets::dwLocalPlayerPawn); // Read local player pointer

            std::uintptr_t CameraServices = mem.Read<std::uintptr_t>(localPlayer + offsets::m_pCameraServices); // Read CameraServices pointer

            std::uintptr_t currentFov = mem.Read<std::uint64_t>(CameraServices + offsets::m_iFOV); // Read current FOV value

            bool isScoped = mem.Read<bool>(localPlayer + offsets::m_bIsScoped); // Check if the player is scoped

            // If the player is not scoped and the current FOV is different from the desired FOV, update it
            if (!isScoped && currentFov != desiredFov)
            {
                mem.Write<uint64_t>(CameraServices + offsets::m_iFOV, desiredFov); // Write the desired FOV to memory
            }

            // std::this_thread::sleep_for(std::chrono::milliseconds(1)); -- save cpu usage.
        }
    }

    // Function to handle visual-related hacks in a separate thread
    void VisualThread(const Memory& mem) noexcept
    {
        while (gui::isRunning) // Keep running while GUI is active
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Sleep for a short duration to reduce CPU usage
        }
    }

    // Function to handle aiming-related hacks in a separate thread
    void AimThread(const Memory& mem) noexcept
    {
        while (gui::isRunning) // Keep running while GUI is active
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Sleep for a short duration to reduce CPU usage
        }
    }

} // namespace hacks
