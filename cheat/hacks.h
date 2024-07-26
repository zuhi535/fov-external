#pragma once
#include "mem.h" // Include the Memory class header for memory operations

namespace hacks
{
    // Function to handle miscellaneous hacks in a separate thread
    // Uses the Memory class to read and write game memory
    void MiscThread(const Memory& mem) noexcept;

    // Function to handle visual-related hacks in a separate thread
    // Uses the Memory class for any visual-related memory operations
    void VisualThread(const Memory& mem) noexcept;

    // Function to handle aiming-related hacks in a separate thread
    // Uses the Memory class to perform operations related to aiming
    void AimThread(const Memory& mem) noexcept;

    // Function to handle ESP (Extra Sensory Perception) hacks in a separate thread
    // Uses the Memory class for operations related to ESP, such as drawing objects or player names
    void ESPThread(const Memory& mem) noexcept;
}
