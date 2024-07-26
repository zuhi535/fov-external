# CS2 External (FOV Changer) Framework

**Note:** This project is provided strictly for educational purposes. Using or distributing cheats for online games is unethical and against the terms of service of most games. This repository is meant to demonstrate how one might interact with a game's memory for educational purposes and should not be used to engage in cheating or other malicious activities.

## Overview

The CS2 Cheat Framework is a simple example of how to interact with a game's memory for modifying gameplay elements, such as Field of View (FOV). This project demonstrates techniques for:

- Reading and writing process memory.
- Creating an in-game overlay with ImGui and DirectX.
- Handling process and module information.

## Features

- **Memory Reading/Writing**: Interact with the memory of the target process to read and modify values.
- **Custom GUI**: Create and manage a custom GUI using ImGui and DirectX 9.
- **Multithreading**: Run multiple threads to handle different aspects of the cheat.

## Components

- **`Memory` Class**: Handles opening the target process, reading, and writing memory.
- **`Globals`**: Stores global variables and offsets for memory access.
- **`GUI`**: Manages the graphical user interface using ImGui and DirectX 9.
- **`Hacks`**: Contains various threads for different cheats or game interactions.

## Setup

### Prerequisites

- **Windows SDK**: Required for Windows API functions.
- **DirectX SDK**: Required for DirectX 9 development.
- **ImGui**: A popular GUI library for creating in-game overlays.

### Building

1. **Clone the Repository**:
    ```sh
    git clone https://github.com/yourusername/cs2-cheat-framework.git
    cd cs2-cheat-framework
    ```

2. **Install Dependencies**:
    - Ensure you have the DirectX SDK installed.
    - Download ImGui and place it in the appropriate directory.

3. **Configure Project**:
    - Set up a new project in your preferred IDE and link against the DirectX and ImGui libraries.
    - Add the source files from this repository to your project.

4. **Compile**:
    - Build the project using your IDE or command line tools.

## Usage

1. **Run the Application**:
    - Launch the application. It will attempt to find and interact with `cs2.exe`.

2. **Adjust Settings**:
    - Use the GUI to adjust the Field of View (FOV) and other settings as needed.

3. **Exit**:
    - Close the GUI window to stop the application.

## Disclaimer

This project is intended for educational purposes only. The usage of cheats and hacks in online games is unethical and against the terms of service of most games. The authors of this project do not condone or encourage cheating in any form. Please use this code responsibly and adhere to the terms of service of the games you play.
