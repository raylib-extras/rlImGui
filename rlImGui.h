/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   rlImGui * basic ImGui integration
*
*   LICENSE: ZLIB
*
*   Copyright (c) 2023 Jeffery Myers
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

#ifndef NO_FONT_AWESOME
#include "extras/IconsFontAwesome6.h"
#define FONT_AWESOME_ICON_SIZE 11
#endif

#ifdef __cplusplus
extern "C" {
#endif

// High level API. This API is designed in the style of raylib and meant to work with reaylib code.
// It will manage it's own ImGui context and call common ImGui functions (like NewFrame and Render) for you
// for a lower level API that matches the other ImGui platforms, please see imgui_impl_raylib.h

/// <summary>
/// Sets up ImGui, loads fonts and themes
/// Calls ImGui_ImplRaylib_Init and sets the theme. Will install Font awesome by default
/// </summary>
/// <param name="darkTheme">when true(default) the dark theme is used, when false the light theme is used</param>
void rlImGuiSetup(bool darkTheme);

/// <summary>
/// Starts a new ImGui Frame
/// Calls ImGui_ImplRaylib_NewFrame, ImGui_ImplRaylib_ProcessEvents, and ImGui::NewFrame together
/// </summary>
void rlImGuiBegin();

/// <summary>
/// Ends an ImGui frame and submits all ImGui drawing to raylib for processing.
/// Calls ImGui:Render, an d ImGui_ImplRaylib_RenderDrawData to draw to the current raylib render target
/// </summary>
void rlImGuiEnd();

/// <summary>
/// Cleanup ImGui and unload font atlas
/// Calls ImGui_ImplRaylib_Shutdown
/// </summary>
void rlImGuiShutdown();

// Advanced StartupAPI

/// <summary>
/// Custom initialization. Not needed if you call rlImGuiSetup. Only needed if you want to add custom setup code.
/// must be followed by rlImGuiEndInitImGui
/// Called by ImGui_ImplRaylib_Init, and does the first part of setup, before fonts are rendered
/// </summary>
void rlImGuiBeginInitImGui();

/// <summary>
/// End Custom initialization. Not needed if you call rlImGuiSetup. Only needed if you want to add custom setup code.
/// must be proceeded by rlImGuiBeginInitImGui
/// Called by ImGui_ImplRaylib_Init and does the second part of setup, and renders fonts.
/// </summary>
void rlImGuiEndInitImGui();

/// <summary>
/// Forces the font texture atlas to be recomputed and re-cached
/// </summary>
void rlImGuiReloadFonts();

// Advanced Update API

/// <summary>
/// Starts a new ImGui Frame with a specified delta time
/// </summary>
/// <param name="dt">delta time, any value < 0 will use raylib GetFrameTime</param>
void rlImGuiBeginDelta(float deltaTime);

// ImGui Image API extensions
// Purely for convenience in working with raylib textures as images.
// If you want to call ImGui image functions directly, simply pass them the pointer to the texture.

/// <summary>
/// Draw a texture as an image in an ImGui Context
/// Uses the current ImGui Cursor position and the full texture size.
/// </summary>
/// <param name="image">The raylib texture to draw</param>
void rlImGuiImage(const Texture *image);

/// <summary>
/// Draw a texture as an image in an ImGui Context at a specific size
/// Uses the current ImGui Cursor position and the specified width and height
/// The image will be scaled up or down to fit as needed
/// </summary>
/// <param name="image">The raylib texture to draw</param>
/// <param name="width">The width of the drawn image</param>
/// <param name="height">The height of the drawn image</param>
void rlImGuiImageSize(const Texture *image, int width, int height);

/// <summary>
/// Draw a texture as an image in an ImGui Context at a specific size
/// Uses the current ImGui Cursor position and the specified size
/// The image will be scaled up or down to fit as needed
/// </summary>
/// <param name="image">The raylib texture to draw</param>
/// <param name="size">The size of drawn image</param>
void rlImGuiImageSizeV(const Texture* image, Vector2 size);

/// <summary>
/// Draw a portion texture as an image in an ImGui Context at a defined size
/// Uses the current ImGui Cursor position and the specified size
/// The image will be scaled up or down to fit as needed
/// </summary>
/// <param name="image">The raylib texture to draw</param>
/// <param name="destWidth">The width of the drawn image</param>
/// <param name="destHeight">The height of the drawn image</param>
/// <param name="sourceRect">The portion of the texture to draw as an image. Negative values for the width and height will flip the image</param>
void rlImGuiImageRect(const Texture* image, int destWidth, int destHeight, Rectangle sourceRect);

/// <summary>
/// Draws a render texture as an image an ImGui Context, automatically flipping the Y axis so it will show correctly on screen
/// </summary>
/// <param name="image">The render texture to draw</param>
void rlImGuiImageRenderTexture(const RenderTexture* image);

/// <summary>
/// Draws a render texture as an image an ImGui Context, automatically flipping the Y axis so it will show correctly on screen
/// Fits the render texture to the available content area
/// </summary>
/// <param name="image">The render texture to draw</param>
/// <param name="center">When true the image will be centered in the content area</param>
void rlImGuiImageRenderTextureFit(const RenderTexture* image, bool center);

/// <summary>
/// Draws a texture as an image button in an ImGui context. Uses the current ImGui cursor position and the full size of the texture
/// </summary>
/// <param name="name">The display name and ImGui ID for the button</param>
/// <param name="image">The texture to draw</param>
/// <returns>True if the button was clicked</returns>
bool rlImGuiImageButton(const char* name, const Texture* image);

/// <summary>
/// Draws a texture as an image button in an ImGui context. Uses the current ImGui cursor position and the specified size.
/// </summary>
/// <param name="name">The display name and ImGui ID for the button</param>
/// <param name="image">The texture to draw</param>
/// <param name="size">The size of the button/param>
/// <returns>True if the button was clicked</returns>
bool rlImGuiImageButtonSize(const char* name, const Texture* image, struct ImVec2 size);

#ifdef __cplusplus
}
#endif
