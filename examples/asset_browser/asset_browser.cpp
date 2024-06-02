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

#define  _CRT_NONSTDC_NO_WARNINGS

#include "asset_browser.h"
#include "imgui_utils.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "raylib.h"
#include "extras/IconsFontAwesome6.h"


AssetBrowserPanel::AssetBrowserPanel()
{
    AssetRoot = GetWorkingDirectory();
    RebuildFolderTree();
    SetCurrentFolder(&FolderRoot);

    CurrentView = &ListView;
}

void AssetBrowserPanel::Show()
{
    ShowHeader();

    ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Borders;
  
    if (ImGui::BeginTable("AssetBrowserTab", 2, flags, ImGui::GetContentRegionAvail()))
    {
        ImGui::TableSetupColumn("FolderView", ImGuiTableColumnFlags_None, 0.25f);
        ImGui::TableSetupColumn("AssetView", ImGuiTableColumnFlags_None, 0.75f);
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        if (ImGui::BeginChild("FolderList", ImGui::GetContentRegionAvail(),ImGuiChildFlags_None, ImGuiWindowFlags_None))
        {
            ShowFolderTree();
            ImGui::EndChild();
        }

        ImGui::TableNextColumn();
        ShowFilePane();
        ImGui::EndTable();
    }
}

void AssetBrowserPanel::RebuildFolderTree()
{
    FolderRoot.Children.clear();

    FolderRoot.FullPath = AssetRoot;
    FolderRoot.Name = GetFileNameWithoutExt(AssetRoot.c_str());
    FolderRoot.Parent = nullptr;
    FolderRoot.Icon = ICON_FA_SERVER;
    FolderRoot.ForceOpenNextFrame = true;
    FolderRoot.PopulateChildren();
}

void AssetBrowserPanel::SetCurrentFolder(FolderInfo* folder)
{
    if (CurrentFolderContents.Folder == folder)
        return;

    CurrentFolderContents.Folder = folder;
    CurrentFolderContents.Files.clear();

    if (folder == nullptr)
        return;

    FolderInfo* openFolder = folder;
    while (openFolder != nullptr)
    {
        openFolder->ForceOpenNextFrame = true;
        openFolder = openFolder->Parent;
    }

    auto files = LoadDirectoryFiles(CurrentFolderContents.Folder->FullPath.c_str());

    for (unsigned int i = 0; i < files.count; i++)
    {
        if (DirectoryExists(files.paths[i]))
            continue;

        const char* name = GetFileName(files.paths[i]);
        if (!name || *name == '.')
            continue;

        FileInfo& file = CurrentFolderContents.Files.emplace_back();
        file.FullPath = files.paths[i];
        file.Name = name;
        file.Icon = GetFileIcon(name);
    }

    UnloadDirectoryFiles(files);
}

void AssetBrowserPanel::FolderInfo::PopulateChildren()
{
    constexpr Color folderColor = { 255,255,145,255 };

    auto folders = LoadDirectoryFiles(FullPath.c_str());

    for (unsigned int i = 0; i < folders.count; i++)
    {
        if (DirectoryExists(folders.paths[i]))
        {
            const char* name = GetFileNameWithoutExt(folders.paths[i]);
            if (!name || *name == '.')
                continue;
  
            FolderInfo& child = Children.emplace_back();
            child.FullPath = folders.paths[i];
            child.Name = name;
            child.Parent = this;
            child.Tint = folderColor;
            child.Icon = ICON_FA_FOLDER;
            child.PopulateChildren();
        }
    }
    UnloadDirectoryFiles(folders);
}

bool AssetBrowserPanel::ShowFolderTreeNode(FolderInfo& info)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (info.Children.empty())
        flags |= ImGuiTreeNodeFlags_Leaf;

    if (CurrentFolderContents.Folder == &info)
        flags |= ImGuiTreeNodeFlags_Selected;

    if (info.ForceOpenNextFrame)
    {
        ImGui::SetNextItemOpen(true);
    }

    bool open = ImGui::TreeNodeEx(info.Name.c_str(), flags, "%s %s", info.Icon.c_str(), info.Name.c_str());

    if (info.ForceOpenNextFrame && CurrentFolderContents.Folder == &info)
        ImGui::ScrollToItem(ImGuiScrollFlags_KeepVisibleCenterY);

    info.ForceOpenNextFrame = false;
    if (ImGui::IsItemClicked())
        SetCurrentFolder(&info);
    
    if (open)
    {
        for (auto& node : info.Children)
            ShowFolderTreeNode(node);

        ImGui::TreePop();
    }

    return CurrentFolderContents.Folder == &info;
}  

void AssetBrowserPanel::ShowHeader()
{
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1);
    if (ImGui::BeginChild("Header", ImVec2{ ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeight() }))
    {
        ImGui::Text("%s Root", ICON_FA_FOLDER_OPEN);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.33f);
        ImGui::InputText("###Path", (char*)(CurrentFolderContents.Folder->FullPath.c_str() + AssetRoot.size()), CurrentFolderContents.Folder->FullPath.size(), ImGuiInputTextFlags_ReadOnly);

        ImGui::EndChild();
    }
    ImGui::PopStyleVar();
}

void AssetBrowserPanel::ShowFolderTree()
{
    ShowFolderTreeNode(FolderRoot);
}

void AssetBrowserPanel::ShowFilePane()
{
    if (ImGui::BeginChild("FileList", ImGui::GetContentRegionAvail(), ImGuiChildFlags_None, ImGuiWindowFlags_None))
    {
        if (CurrentView)
        {
            auto *item = CurrentView->Show(CurrentFolderContents);
            if (item)
            {
                AssetItemInfo* assetItem = static_cast<AssetItemInfo*>(item);
                if (!assetItem->IsFile())
                    SetCurrentFolder(static_cast<FolderInfo*>(item));
            }
        }
        ImGui::EndChild();
    }
}

ViewableItem* AssetBrowserPanel::AssetContainer::Reset()
{
    FileItr = Files.begin();
    FolderItr = Folder->Children.begin();
    if (FileItr != Files.end())
        return &(*FileItr);
    if (FolderItr != Folder->Children.end())
        return &(*FolderItr);

    return nullptr;
}

size_t AssetBrowserPanel::AssetContainer::Count()
{
    return Files.size() + Folder->Children.size();
}

ViewableItem* AssetBrowserPanel::AssetContainer::Next()
{
    if (FileItr != Files.end())
    {
        FileItr++;
        if (FileItr != Files.end())
            return &(*FileItr);
        else
        {
            if (FolderItr != Folder->Children.end())
                return &(*FolderItr);
            else
                return nullptr;
        }
    }

    if (FolderItr != Folder->Children.end())
    {
        FolderItr++;
        if (FolderItr != Folder->Children.end())
            return &(*FolderItr);
    }

    return nullptr;
}

const char* AssetBrowserPanel::GetFileIcon(const char* filename)
{
    const char* ext = GetFileExtension(filename);

    if (ext != nullptr)
    {
        if (stricmp(ext, ".png") == 0)
            return ICON_FA_FILE_IMAGE;

        if (stricmp(ext, ".wav") == 0 || stricmp(ext, ".mp3") == 0 || stricmp(ext, ".oog") == 0)
            return ICON_FA_FILE_AUDIO;

        if (stricmp(ext, ".ttf") == 0 || stricmp(ext, ".otf") == 0 || stricmp(ext, ".fnt") == 0)
            return ICON_FA_FONT;

        if (stricmp(ext, ".txt") == 0 || stricmp(ext, ".md") == 0)
            return ICON_FA_FILE_LINES;

        if (stricmp(ext, ".lua") == 0 || stricmp(ext, ".c") == 0 || stricmp(ext, ".h") == 0 || stricmp(ext, ".cpp") == 0)
            return ICON_FA_FILE_CODE;
    }
    return ICON_FA_FILE;
}