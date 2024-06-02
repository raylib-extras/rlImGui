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

#include "item_view.h"
#include "raylib.h"

#include <string>
#include <vector>
#include <list>

class AssetBrowserPanel
{
public:
    AssetBrowserPanel();

    void Show();

private:
    std::string AssetRoot;

    class AssetItemInfo : public ViewableItem
    {
    protected:
        bool File = false;

    public:
        AssetItemInfo(bool file) : File(file) {}
        bool IsFile() const { return File; }
    };

    class FileInfo : public AssetItemInfo
    {
    public:
        FileInfo() : AssetItemInfo(true) {}

        std::string FullPath;
    };

    class FolderInfo : public AssetItemInfo
    {
    public:
        FolderInfo() : AssetItemInfo(false) {}

        std::string FullPath;
        FolderInfo* Parent = nullptr;
        std::list<FolderInfo> Children;

        bool ForceOpenNextFrame = false;

        void PopulateChildren();
    };

    FolderInfo FolderRoot;

    class AssetContainer : public ViewableItemContainer
    {
    public:
        ViewableItem* Reset() override;
        size_t Count() override;
        ViewableItem* Next() override;

        FolderInfo* Folder = nullptr;
        std::vector<FileInfo> Files;

        std::vector<FileInfo>::iterator FileItr;
        std::list<FolderInfo>::iterator FolderItr;
    };

    AssetContainer CurrentFolderContents;

    ListItemView ListView;

    ItemView* CurrentView = nullptr;

    void RebuildFolderTree();

    void SetCurrentFolder(FolderInfo* folder);

    bool ShowFolderTreeNode(FolderInfo& folder);
    void ShowFolderTree();
    void ShowFilePane();
    void ShowHeader();

    const char* GetFileIcon(const char* filename);
};