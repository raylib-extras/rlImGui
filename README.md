# rlImGui
<img align="left" src="https://github.com/raysan5/raylib/raw/master/logo/raylib_logo_animation.gif" width="64">
A Raylib integration with DearImGui

rlImgui provides a backend for [Dear ImGui](https://github.com/ocornut/imgui) using [Raylib](https://www.raylib.com/). 

# Building
The rlImGui repository itself is set up to use Premake to generate a static library and examples for Visual Studio 2019. Premake can also be used to generate makefiles for Linux. rlImGui can be used as a static library, or by directly including the files into your game project.
Premake is not required to use rlImGui, it is simply just what is used for development.

## Other Systems
rlImGui has no dependencies other than raylib and imgui. If you want to use any other build system, you can simply just build rlImGui and ImGui into a library, or even add the files direclty to your game if it can use C++ code. There are no specific build requirements for rlImgui itself.

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
bool rlImGuiImageButtonSize(const char* name, const Texture* image, Vector2 size);
```

# C vs C++
ImGui is a C++ library, so rlImGui uses C++ to create the backend and integration with Raylib.
The rlImGui.h API only uses features that are common to C and C++, so rlImGui can be built as a static library and used by pure C code. Users of ImGui who wish to use pure C must use an ImGui wrapper, such as [https://github.com/cimgui/cimgui].

# Low level API
If you would like more controll over the ImGui Backend, you can use the low level API that is found in imgui_impl_raylib.h. This is API follows the patterns of other ImGui backends and does not do automatic context management. An example of it's use can be found in imgui_style_example.cpp 

# Note for High DPI displays
If your system does a display scale, like 125% or %150, you will write code to handle that.
If you set the FLAG_WINDOW_HIGHDPI flag in raylib, that will create a frame buffer that is automatically scaled to fit your display. This makes it easy to define all your code in a 'normal' resolution, but has the disadvantage of making it harder to define other bufers in the native resolution. The most common side effect of this is that fonts look blury, because they are rendered at the non scaled resolution.
rlImGui on non-apple platforms will scale the default fonts by the display scale to compensate, but if you have your own fonts, you will need to do the same.
Note that apple platforms have several driver bugs with high DPI (retena displays), and rlImGui tries to compensate for them, but your results may vary or be inconsistent with other platforms.

The better option is to not use FLAG_WINDOW_HIGHDPI and let raylib run in the native resolution. You should then scale all your input values by GetWindowDPIScale.

Some examples show how to scale hardcoded values by the display scale to compensate and make your GUI look good in any scale.
