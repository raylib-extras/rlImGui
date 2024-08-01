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


#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"
#include "extras/FA6FreeSolidFontData.h"

#include "asset_browser.h"

#include <limits>


ImFont* IconFont = nullptr;

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 1900;
	int screenHeight = 900;

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(screenWidth, screenHeight, "raylib-Extras [ImGui] example - Asset browser");
	SetTargetFPS(144);

    rlImGuiBeginInitImGui();
    ImGui::StyleColorsDark();


    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.FontDataOwnedByAtlas = false;

    icons_config.GlyphMaxAdvanceX = std::numeric_limits<float>::max();
    icons_config.RasterizerMultiply = 1.0f;
    icons_config.OversampleH = 2;
    icons_config.OversampleV = 1;

    icons_config.GlyphRanges = icons_ranges;

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromMemoryCompressedTTF((void*)fa_solid_900_compressed_data, fa_solid_900_compressed_size, 12, &icons_config, icons_ranges);

    icons_config.MergeMode = false;
    IconFont = io.Fonts->AddFontFromMemoryCompressedTTF((void*)fa_solid_900_compressed_data, fa_solid_900_compressed_size, 72, &icons_config, icons_ranges);

    rlImGuiEndInitImGui();

	AssetBrowserPanel assetBrowser;
	
	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{

		BeginDrawing();
		ClearBackground(DARKGRAY);

		rlImGuiBegin();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(float(GetScreenWidth()), float(GetScreenHeight())));
		if (ImGui::Begin("Frame", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings))
		{
			assetBrowser.Show();
		}
		ImGui::End();

		rlImGuiEnd();

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	rlImGuiShutdown();

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}