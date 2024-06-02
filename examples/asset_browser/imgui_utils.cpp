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

#include "imgui_utils.h"

#include "imgui.h"
#include "imgui_internal.h"

namespace ImGuiUtils
{

    bool IsSpace(char aCharacter)
    {
        // all space characters are values 32 or less (space is 32)
        // so we can convert them to a bitmask and use a single condition
        const int mask = (1 << (' ' - 1)) | (1 << ('\f' - 1)) | (1 << ('\n' - 1)) | (1 << ('\r' - 1)) | (1 << ('\t' - 1)) | (1 << ('\v' - 1));
        return (mask & (1 << ((aCharacter && aCharacter <= 32) * (aCharacter - 1)))) != 0;
    }

    //-------------------------------------------------------------------------------------------------
    // Todo: Add support for soft-hyphens when using word boundaries?
    //-------------------------------------------------------------------------------------------------
    void TextWithEllipsis(const char* string, float aMaxWidth, bool useWordBoundaries, float aSpacing)
    {
        char const* partStart = string;
        char const* partEnd = string;

        ImWchar elipsisChar = ImGui::GetFont()->EllipsisChar;
        char elipsisText[8];
        ImTextStrToUtf8(elipsisText, sizeof(elipsisText), &elipsisChar, (&elipsisChar) + 1);

        if (aSpacing < 0.0f) aSpacing = ImGui::GetStyle().ItemSpacing.x;

        float const ellipsisWidth = ImGui::CalcTextSize(elipsisText).x + aSpacing;
        float width = 0;
        bool addElipsis = false;

        while (*partStart != 0 )
        {
            // Add space to next segment
            while (IsSpace(*partEnd))
                partEnd++;

            if (useWordBoundaries)
            {
                // get next 'word' by looking for space after non-space
                while (*partEnd != 0 && !IsSpace(*partEnd))
                    ++partEnd;
            }
            else
            {
                if (*partEnd != 0)
                    ++partEnd;
            }

            ImVec2 const wordSize = ImGui::CalcTextSize(partStart, partEnd);

            // Clearly we have space for this word so just add it
            if (wordSize.x + width + ellipsisWidth < aMaxWidth)
            {
                width += wordSize.x;
                partStart = partEnd;
            }
            // If we're just at the end of the word and we just fit then we can commit here
            else if (*partEnd == 0 && wordSize.x + width < aMaxWidth)
            {
                width += wordSize.x;
                partStart = partEnd;
            }
            // we're done so add elipsis where the current segment starts
            else
            {
                addElipsis = true;
                break;
            }
        }

        ImGui::TextUnformatted(string, partStart);

        if (addElipsis)
        {
            ImGui::SameLine(0.0f, aSpacing);
            ImGui::TextUnformatted(elipsisText);
        }
    }
}