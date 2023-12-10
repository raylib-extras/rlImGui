# rlImGui
<img align="left" src="https://github.com/raysan5/raylib/raw/master/logo/raylib_logo_animation.gif" width="64">
A Raylib integration with DearImGui

rlImgui provides a backend for [Dear ImGui](https://github.com/ocornut/imgui) using [Raylib](https://www.raylib.com/). 

# Building
rlImGui is setup to use premake to generate a static library and examples for Visual Studio 2019. Premake can also be used to generate makefiles for linux. rlImGui can be used as a static library, or by direclty including the files into your game project.

If you wish to use premake, you will need to download the Premake5 executable for your platform from. https://premake.github.io/download

# Setup

Using rlImGui in your code is very easy. Once you have included the library, or source files for rlImGui and ImGui in your project, simply do the following.
```
#include "rlImGui.h"	// include the API header

// before your game loop
rlImGuiSetup(true); 	// sets up ImGui with ether a dark or light default theme

// inside your game loop, between BeginDrawing() and EndDrawing()
rlImGuiBegin();			// starts the ImGui content mode. Make all ImGui calls after this

rlImGuiEnd();			// ends the ImGui content mode. Make all ImGui calls before this

// after your game loop is over, before you close the window

rlImGuiShutdown();		// cleans up ImGui
```

# Examples
There are two example programs in the examples folder.

## Simple
This is the most simple use of ImGui in raylib, it just shows the ImGui demo window.
![image](https://user-images.githubusercontent.com/322174/136596910-da1b60ae-4a39-48f0-ae84-f568bc396870.png)


## Editor
This is a more complex example of ImGui, showing how to use raylib 2d and 3d cameras to draw into ImGui windows using render textures.
![image](https://user-images.githubusercontent.com/322174/136596949-033ffe0a-2476-4030-988a-5bf5b6e2ade7.png)

# Extras

## rlImGuiColors.h
This file has a converter to change Raylib colors into ImGui Colors

## Font Awesome Icons
Support for Font Awesome 6 https://fontawesome.com/ is built into rlImGui and enabled by default. You can simply
#include "extras/IconsFontAwesome6.h"
To use the ICON_FA macros for any icon in the free set.

If you wish to disable font awesome support you can #define NO_FONT_AWESOME


# Images
Raylib textures can be drawn in ImGui using the following functions
```
void rlImGuiImage(const Texture *image);
void rlImGuiImageSize(const Texture *image, int width, int height);
void rlImGuiImageSizeV(const Texture* image, Vector2 size);
void rlImGuiImageRect(const Texture* image, int destWidth, int destHeight, Rectangle sourceRect);
void rlImGuiImageRenderTexture(const RenderTexture* image);
void rlImGuiImageRenderTextureFit(const RenderTexture* image, bool center);

bool rlImGuiImageButton(const Texture *image);
bool rlImGuiImageButtonSize(const char* name, const Texture* image, struct ImVec2 size);
```

# C vs C++
ImGui is a C++ library, so rlImGui uses C++ to create the backend and integration with Raylib.
The rlImGui.h API only uses features that are common to C and C++, so rlImGui can be built as a static library and used by pure C code. Users of ImGui who wish to use pure C must use an ImGui wrapper, such as [https://github.com/cimgui/cimgui].

# Low level API
If you would like more controll over the ImGui Backend, you can use the low level API that is found in imgui_impl_raylib.h. This is API follows the patterns of other ImGui backends and does not do automatic context management. An example of it's use can be found in imgui_style_example.cpp 
