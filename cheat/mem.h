#pragma once
#define WIN32_LEAN_AND_MEAN // Excludes rarely-used stuff from Windows headers to speed up compilation
#include <Windows.h>       // Windows API functions and types
#include <TlHelp32.h>      // Toolhelp32 API functions and types

#include <cstdint>          // Fixed-width integer types
#include <string_view>     // Non-owning string view

// Memory class provides methods for reading and writing process memory
class Memory
{
private:
    std::uintptr_t processId = 0;      // Process ID of the target process
    void* processHandle = nullptr;    // Handle to the target process

public:
    // Constructor that initializes the Memory object by finding the process ID
    // and opening a handle to the process
    Memory(const std::string_view processName) noexcept
    {
        ::PROCESSENTRY32 entry = { };
        entry.dwSize = sizeof(::PROCESSENTRY32);  // Initialize PROCESSENTRY32 structure

        // Create a snapshot of all processes
        const auto snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        // Iterate through the processes in the snapshot
        while (::Process32Next(snapShot, &entry))
        {
            // Compare process name with the provided name
            if (!processName.compare(entry.szExeFile))
            {
                processId = entry.th32ProcessID;             // Store the process ID
                processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId); // Open process handle
                break; // Exit loop once the target process is found
            }
        }

        // Close the snapshot handle
        if (snapShot)
            ::CloseHandle(snapShot);
    }

    // Destructor that closes the handle to the process
    ~Memory()
    {
        if (processHandle)
            ::CloseHandle(processHandle); // Release the process handle
    }

    // Returns the base address of a module by its name
    const std::uintptr_t GetModuleAddress(const std::string_view moduleName) const noexcept
    {
        ::MODULEENTRY32 entry = { };
        entry.dwSize = sizeof(::MODULEENTRY32);  // Initialize MODULEENTRY32 structure

        // Create a snapshot of modules in the target process
        const auto snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);

        std::uintptr_t result = 0;

        // Iterate through the modules in the snapshot
        while (::Module32Next(snapShot, &entry))
        {
            // Compare module name with the provided name
            if (!moduleName.compare(entry.szModule))
            {
                result = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr); // Get the base address
                break; // Exit loop once the target module is found
            }
        }

        // Close the snapshot handle
        if (snapShot)
            ::CloseHandle(snapShot);

        return result; // Return the base address of the module
    }

    // Template function to read data from process memory
    template <typename T>
    constexpr const T Read(const std::uintptr_t& address) const noexcept
    {
        T value = { }; // Initialize value
        // Read memory from the specified address into value
        ::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
        return value; // Return the read value
    }

    // Template function to write data to process memory
    template <typename T>
    constexpr void Write(const std::uintptr_t& address, const T& value) const noexcept
    {
        // Write value to the specified address in process memory
        ::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
    }
};
