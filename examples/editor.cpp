/*******************************************************************************************
*
*   raylib-extras [ImGui] example - editor
*
*	This is a more complex ImGui Integration
*	It shows how to build windows on top of 2d and 3d views using a render texture
*
*   Copyright (c) 2021 Jeffery Myers
*
********************************************************************************************/


#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"

bool Quit = false;

bool ImGuiDemoOpen = false;

class DocumentWindow
{
public:
	bool Open = false;

	RenderTexture ViewTexture;

	virtual void Setup() = 0;
	virtual void Shutdown() = 0;
	virtual void Show() = 0;
	virtual void Update() = 0;

	bool Focused = false;

	Rectangle ContentRect = { 0 };
};

class ImageViewerWindow : public DocumentWindow
{
public:

	void Setup() override
	{
		Camera.zoom = 1;
		Camera.target.x = 0;
		Camera.target.y = 0;
		Camera.rotation = 0;
		Camera.offset.x = GetScreenWidth() / 2.0f;
		Camera.offset.y = GetScreenHeight() / 2.0f;

		ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
		ImageTexture = LoadTexture("resources/parrots.png");

		UpdateRenderTexture();
	}

	void Show() override
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

		Focused = false;

		if (ImGui::Begin("Image Viewer", &Open, ImGuiWindowFlags_NoScrollbar))
		{
			// save off the screen space content rectangle
			ContentRect = { ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y, ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

			Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

			ImVec2 size = ImGui::GetContentRegionAvail();

			// center the scratch pad in the view
			Rectangle viewRect = { 0 };
			viewRect.x = ViewTexture.texture.width / 2 - size.x / 2;
			viewRect.y = ViewTexture.texture.height / 2 - size.y / 2;
			viewRect.width = size.x;
			viewRect.height = -size.y;

			if (ImGui::BeginChild("Toolbar", ImVec2(ImGui::GetContentRegionAvail().x, 25)))
			{
				ImGui::SetCursorPosX(2);
				ImGui::SetCursorPosY(3);

				if (ImGui::Button("None"))
				{
					CurrentToolMode = ToolMode::None;
				}
				ImGui::SameLine();

				if (ImGui::Button("Move"))
				{
					CurrentToolMode = ToolMode::Move;
				}

				ImGui::SameLine();
				switch (CurrentToolMode)
				{
					case ToolMode::None:
						ImGui::TextUnformatted("No Tool");
						break;
					case ToolMode::Move:
						ImGui::TextUnformatted("Move Tool");
						break;
					default:
						break;
				}

				ImGui::SameLine();
				ImGui::TextUnformatted(TextFormat("camera target X%f Y%f", Camera.target.x, Camera.target.y));
				ImGui::EndChild();
			}

			rlImGuiImageRect(&ViewTexture.texture, (int)size.x, (int)size.y, viewRect);	
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Update() override
	{
		if (!Open)
			return;

		if (IsWindowResized())
		{
			UnloadRenderTexture(ViewTexture);
			ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

			Camera.offset.x = GetScreenWidth() / 2.0f;
			Camera.offset.y = GetScreenHeight() / 2.0f;
		}

		Vector2 mousePos = GetMousePosition();

		if (Focused)
		{
			if (CurrentToolMode == ToolMode::Move)
			{
				// only do this tool when the mouse is in the content area of the window
				if (IsMouseButtonDown(0) && CheckCollisionPointRec(mousePos, ContentRect))
				{
					if (!Dragging)
					{
						LastMousePos = mousePos;
						LastTarget = Camera.target;
					}
					Dragging = true;
					Vector2 mouseDelta = Vector2Subtract(LastMousePos, mousePos);

					mouseDelta.x /= Camera.zoom;
					mouseDelta.y /= Camera.zoom;
					Camera.target = Vector2Add(LastTarget, mouseDelta);

					DirtyScene = true;

				}
				else
				{
					Dragging = false;
				}
			}
		}
		else
		{
			Dragging = false;
		}

		if (DirtyScene)
		{
			DirtyScene = false;
			UpdateRenderTexture();
		}
	}

	Texture ImageTexture;
	Camera2D Camera = { 0 };

	Vector2 LastMousePos = { 0 };
	Vector2 LastTarget = { 0 };
	bool Dragging = false;

	bool DirtyScene = false;

	enum class ToolMode
	{
		None,
		Move,
	};

	ToolMode CurrentToolMode = ToolMode::None;

	void UpdateRenderTexture()
	{
		BeginTextureMode(ViewTexture);
		ClearBackground(BLUE);

		// camera with our view offset with a world origin of 0,0
		BeginMode2D(Camera);

		// center the image at 0,0
		DrawTexture(ImageTexture, ImageTexture.width / -2, ImageTexture.height / -2, WHITE);

		EndMode2D();
		EndTextureMode();
	}

	void Shutdown() override
	{
		UnloadRenderTexture(ViewTexture);
		UnloadTexture(ImageTexture);
	}
};

class SceneViewWindow : public DocumentWindow
{
public:
	Camera3D Camera = { 0 };

	void Setup() override
	{
		ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

		Camera.fovy = 45;
		Camera.up.y = 1;
		Camera.position.y = 3;
		Camera.position.z = -25;

		Image img = GenImageChecked(256, 256, 32, 32, DARKGRAY, WHITE);
		GridTexture = LoadTextureFromImage(img);
		UnloadImage(img);
		GenTextureMipmaps(&GridTexture);
		SetTextureFilter(GridTexture, TEXTURE_FILTER_ANISOTROPIC_16X);
		SetTextureWrap(GridTexture, TEXTURE_WRAP_CLAMP);
	}

	void Shutdown() override
	{
		UnloadRenderTexture(ViewTexture);
		UnloadTexture(GridTexture);
	}

	void Show() override
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

		if (ImGui::Begin("3D View", &Open, ImGuiWindowFlags_NoScrollbar))
		{
			Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
			// draw the view
			rlImGuiImageRenderTextureFit(&ViewTexture, true);
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Update() override
	{
		if (!Open)
			return;

		if (IsWindowResized())
		{
			UnloadRenderTexture(ViewTexture);
			ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
		}

		float period = 10;
		float magnitude = 25;

		Camera.position.x = (float)(sinf((float)GetTime() / period) * magnitude);

		BeginTextureMode(ViewTexture);
		ClearBackground(SKYBLUE);

		BeginMode3D(Camera);

		// grid of cube trees on a plane to make a "world"
		DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 50, 50 }, BEIGE); // simple world plane
		float spacing = 4;
		int count = 5;

		for (float x = -count * spacing; x <= count * spacing; x += spacing)
		{
			for (float z = -count * spacing; z <= count * spacing; z += spacing)
			{
				Vector3 pos = { x, 0.5f, z };

				Vector3 min = { x - 0.5f,0,z - 0.5f };
				Vector3 max = { x + 0.5f,1,z + 0.5f };

				DrawCube(Vector3{ x, 1.5f, z }, 1, 1, 1, GREEN);
				DrawCube(Vector3{ x, 0.5f, z }, 0.25f, 1, 0.25f, BROWN);
			}
		}

		EndMode3D();
		EndTextureMode();
	}

	Texture2D GridTexture = { 0 };
};


ImageViewerWindow ImageViewer;
SceneViewWindow SceneView;

void DoMainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
				Quit = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("ImGui Demo", nullptr, &ImGuiDemoOpen);
			ImGui::MenuItem("Image Viewer", nullptr, &ImageViewer.Open);
			ImGui::MenuItem("3D View", nullptr, &SceneView.Open);

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 1900;
	int screenHeight = 900;

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(screenWidth, screenHeight, "raylib-Extras [ImGui] example - ImGui Demo");
	SetTargetFPS(144);
	rlImGuiSetup(true);
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

	ImageViewer.Setup();
	ImageViewer.Open = true;

	SceneView.Setup();
	SceneView.Open = true;

	// Main game loop
	while (!WindowShouldClose() && !Quit)    // Detect window close button or ESC key
	{
		ImageViewer.Update();
		SceneView.Update();

		BeginDrawing();
		ClearBackground(DARKGRAY);

		rlImGuiBegin();
		DoMainMenu();

		if (ImGuiDemoOpen)
			ImGui::ShowDemoWindow(&ImGuiDemoOpen);

		if (ImageViewer.Open)
			ImageViewer.Show();

		if (SceneView.Open)
			SceneView.Show();

		rlImGuiEnd();

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	rlImGuiShutdown();

	ImageViewer.Shutdown();
	SceneView.Shutdown();

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}