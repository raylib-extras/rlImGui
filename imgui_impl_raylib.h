/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   rlImGui * basic ImGui integration
*
*   LICENSE: ZLIB
*   Copyright (c) 2020-2021 Jeffery Myers
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*

**********************************************************************************************/

// dear imgui: Platform Backend for Raylib
// (Info: Raylib is a cross-platform general purpose library for handling windows, inputs, graphics context creation, etc. using OpenGL)
// This is is the low level ImGui backend for raylib, a higher level API that matches the raylib API can be found in rlImGui.h

// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#pragma once
#include "imgui.h"      // IMGUI_IMPL_API
#ifndef IMGUI_DISABLE

IMGUI_IMPL_API bool ImGui_ImplRaylib_Init(void);
IMGUI_IMPL_API void ImGui_ImplRaylib_Shutdown(void);
IMGUI_IMPL_API void ImGui_ImplRaylib_NewFrame(void);
IMGUI_IMPL_API void ImGui_ImplRaylib_RenderDrawData(ImDrawData* draw_data);
IMGUI_IMPL_API bool ImGui_ImplRaylib_ProcessEvents(void);

#endif // #ifndef IMGUI_DISABLE
