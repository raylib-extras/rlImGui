/*******************************************************************************************
*
*   raylib-extras [ImGui] example - Docking example
*
*	This is an example of using the ImGui docking features that are part of docking branch
*	You must replace the default imgui with the code from the docking branch for this to work
*	https://github.com/ocornut/imgui/tree/docking
*
*   Copyright (c) 2024 Jeffery Myers
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"


int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 1280;
	int screenHeight = 800;

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "raylib-Extras [ImGui] example - Docking");
	SetTargetFPS(144);
	rlImGuiSetup(true);

	bool run = true;

	bool showDemoWindow = true;

	// if the linked ImGui has docking, enable it.
	// this will only be true if you use the docking branch of ImGui.
#ifdef IMGUI_HAS_DOCK
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif

	// Main game loop
	while (!WindowShouldClose() && run)    // Detect window close button or ESC key, or a quit from the menu
	{
		BeginDrawing();
		ClearBackground(DARKGRAY);

		// start ImGui content
		rlImGuiBegin();

		// if you want windows to dock to the viewport, call this.
#ifdef IMGUI_HAS_DOCK
		ImGui::DockSpaceOverViewport();
#endif

		// show a simple menu bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Quit"))
					run = false;

				ImGui::EndMenu();
            }

			if (ImGui::BeginMenu("Window"))
            {
                if (ImGui::MenuItem("Demo Window", nullptr, showDemoWindow))
					showDemoWindow = !showDemoWindow;

                ImGui::EndMenu();
            }
			ImGui::EndMainMenuBar();
		}

		// show some windows
	
		if (showDemoWindow)
			ImGui::ShowDemoWindow(&showDemoWindow);

		if (ImGui::Begin("Test Window"))
		{
			ImGui::TextUnformatted("Another window");
		}
		ImGui::End();

		// end ImGui Content
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