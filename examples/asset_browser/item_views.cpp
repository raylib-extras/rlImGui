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

#include "item_view.h"

#include "imgui.h"
#include "imgui_utils.h"
#include "rlImGuiColors.h"
#include "raylib.h"

extern ImFont* IconFont;

ViewableItem* ListItemView::Show(ViewableItemContainer& container)
{
    ViewableItem* item = container.Reset();

    ViewableItem* selected = nullptr;
    while (item)
    {
        float x = ImGui::GetCursorPosX();

        const char* name = TextFormat("###%s", item->Name.c_str());
        if (item->Tint.a > 0)
            ImGui::TextColored(rlImGuiColors::Convert(item->Tint), " %s", item->Icon.c_str());
        else
            ImGui::Text(" %s", item->Icon.c_str());

        ImGui::SameLine(0, 0);
        ImGui::Text(" %s", item->Name.c_str());
        ImGui::SameLine(0, 0);

        ImGui::SetCursorPosX(x);
        //ImGui::SetItemAllowOverlap();

        ImGui::Selectable(name);
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
        {
            selected = item;
        }

        item = container.Next();
    }

    return selected;
}