/*******************************************************************************************
*
*   raylib-extras [ImGui] example - asset browser
*
*	This is a more complex ImGui Integration
*	It shows how to build windows on top of 2d and 3d views using a render texture
*
*   Copyright (c) 2024 Jeffery Myers
*
********************************************************************************************/

#pragma once

#include <string>

namespace ImGuiUtils
{
    void TextWithEllipsis(const char* string, float maxWidth, bool useWordBoundaries = false, float aSpacing = 0);
}