# rlImGui
<img align="left" src="./assets/raylib_logo.png" width="64">
A Raylib integration with DearImGui

rlImgui provides a backend for [Dear ImGui](https://github.com/ocornut/imgui) using [Raylib](https://www.raylib.com/). 

# Setup

Using rlImGui in your code is very easy. Once you have included the library, or source files for rlImGui and ImGui in your project, simply do the following.

#include "rlImGui.h"	// include the API header

// before your game loop
rlImGuiSetup(true); 	// sets up ImGui with ether a dark or light default theme

// inside your game loop, between BeginDrawing() and EndDrawing()
rlImGuiBegin();			// starts the ImGui content mode. Make all ImGui calls after this

rlImGuiEnd();			// ends the ImGui content mode. Make all ImGui calls before this


// after your game loop is over, before you close the window

rlImGuiShutdown();		// cleans up ImGui

# Examples
There are two example programs in the examples folder.

## Simple
This is the most simple use of ImGui in raylib, it just shows the ImGui demo window.

## Editor
This is a more complex example of ImGui, showing how to use raylib 2d and 3d cameras to draw into ImGui windows using render textures.

# C vs C++
ImGui is a C++ library, so rlImGui uses C++ to create the backend and integration with Raylib.
The rlImGui.h API only uses features that are common to C and C++, so rlImGui can be built as a static library and used by pure C code. Users of ImGui who wish to use pure C must use an ImGui wrapper, such as [https://github.com/cimgui/cimgui].


