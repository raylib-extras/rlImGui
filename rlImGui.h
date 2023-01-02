/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   rlImGui * basic ImGui integration
*
*   LICENSE: ZLIB
*
*   Copyright (c) 2020 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#pragma once

#include "raylib.h"

#include "imgui.h"

#ifndef NO_FONT_AWESOME
#include "extras/IconsFontAwesome6.h"
#define FONT_AWESOME_ICON_SIZE 11
#endif

#ifdef __cplusplus
extern "C" {
#endif

// basic API
void rlImGuiSetup(bool dark);
void rlImGuiBegin();
void rlImGuiEnd();
void rlImGuiShutdown();

// Advanced StartupAPI
void rlImGuiBeginInitImGui();
void rlImGuiEndInitImGui();
void rlImGuiReloadFonts();

// image API
void rlImGuiImage(const Texture *image);
bool rlImGuiImageButton(const char* name, const Texture *image);
bool rlImGuiImageButtonSize(const char* name, const Texture* image, ImVec2 size);
void rlImGuiImageSize(const Texture *image, int width, int height);
void rlImGuiImageRect(const Texture* image, int destWidth, int destHeight, Rectangle sourceRect);

#ifdef __cplusplus
}
#endif
