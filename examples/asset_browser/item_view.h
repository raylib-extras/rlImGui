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
#include "raylib.h"

class ViewableItem
{
public:
    virtual ~ViewableItem() = default;

    std::string Name;
    std::string Icon;
    Color Tint = BLANK;
};

class ViewableItemContainer
{
public:
    virtual ~ViewableItemContainer() = default;
    virtual ViewableItem* Reset() = 0;
    virtual size_t Count() = 0;
    virtual ViewableItem* Next() = 0;
};

class ItemView
{
public:
    virtual ~ItemView() = default;
    virtual ViewableItem* Show(ViewableItemContainer& container) = 0;
};

class ListItemView : public ItemView
{
public:
    ViewableItem* Show(ViewableItemContainer& container) override;
};
