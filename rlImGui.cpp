/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   rlImGui * basic ImGui integration
*
*   LICENSE: ZLIB
*
*   Copyright (c) 2020 Jeffery Myers
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
#include "rlImGui.h"

#include "imgui.h"
#include "raylib.h"
#include "rlgl.h"

#ifdef PLATFORM_DESKTOP
#include <GLFW/glfw3.h>
#endif

#include <math.h>

#ifndef NO_FONT_AWESOME
#include "extras/FA6FreeSolidFontData.h"
#endif

static Texture2D FontTexture;

static ImGuiMouseCursor CurrentMouseCursor = ImGuiMouseCursor_COUNT;
static MouseCursor MouseCursorMap[ImGuiMouseCursor_COUNT];

static const char* rlImGuiGetClipText(void*)
{
	return GetClipboardText();
}

static void rlImGuiSetClipText(void*, const char* text)
{
	SetClipboardText(text);
}

static void rlImGuiNewFrame()
{
	ImGuiIO& io = ImGui::GetIO();

	if (IsWindowFullscreen())
	{
		int monitor = GetCurrentMonitor();
		io.DisplaySize.x = float(GetMonitorWidth(monitor));
		io.DisplaySize.y = float(GetMonitorHeight(monitor));
	}
	else
	{
		io.DisplaySize.x = float(GetScreenWidth());
		io.DisplaySize.y = float(GetScreenHeight());
	}

	int width = int(io.DisplaySize.x), height = int(io.DisplaySize.y);
#ifdef PLATFORM_DESKTOP
	glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
#endif
	if (width > 0 && height > 0) {
		io.DisplayFramebufferScale = ImVec2(width / io.DisplaySize.x, height / io.DisplaySize.y);
	}
	else {
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	}

	io.DeltaTime = GetFrameTime();

	if (io.WantSetMousePos)
	{
		SetMousePosition((int)io.MousePos.x, (int)io.MousePos.y);
	}
	else
	{
		io.MousePos.x = (float)GetMouseX();
		io.MousePos.y = (float)GetMouseY();
	}

	io.MouseDown[0] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
	io.MouseDown[1] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
	io.MouseDown[2] = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);

	if (GetMouseWheelMove() > 0)
		io.MouseWheel += 1;
	else if (GetMouseWheelMove() < 0)
		io.MouseWheel -= 1;

	if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) == 0)
	{
		ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		if (imgui_cursor != CurrentMouseCursor || io.MouseDrawCursor)
		{
			CurrentMouseCursor = imgui_cursor;
			if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
			{
				HideCursor();
			}
			else
			{
				ShowCursor();

				if (!(io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange))
				{
					SetMouseCursor((imgui_cursor > -1 && imgui_cursor < ImGuiMouseCursor_COUNT) ? MouseCursorMap[imgui_cursor] : MOUSE_CURSOR_DEFAULT);
				}
			}
		}
	}
}

#define FOR_ALL_KEYS(X) \
    do { \
    X(KEY_APOSTROPHE); \
    X(KEY_COMMA); \
    X(KEY_MINUS); \
    X(KEY_PERIOD); \
    X(KEY_SLASH); \
    X(KEY_ZERO); \
    X(KEY_ONE); \
    X(KEY_TWO); \
    X(KEY_THREE); \
    X(KEY_FOUR); \
    X(KEY_FIVE); \
    X(KEY_SIX); \
    X(KEY_SEVEN); \
    X(KEY_EIGHT); \
    X(KEY_NINE); \
    X(KEY_SEMICOLON); \
    X(KEY_EQUAL); \
    X(KEY_A); \
    X(KEY_B); \
    X(KEY_C); \
    X(KEY_D); \
    X(KEY_E); \
    X(KEY_F); \
    X(KEY_G); \
    X(KEY_H); \
    X(KEY_I); \
    X(KEY_J); \
    X(KEY_K); \
    X(KEY_L); \
    X(KEY_M); \
    X(KEY_N); \
    X(KEY_O); \
    X(KEY_P); \
    X(KEY_Q); \
    X(KEY_R); \
    X(KEY_S); \
    X(KEY_T); \
    X(KEY_U); \
    X(KEY_V); \
    X(KEY_W); \
    X(KEY_X); \
    X(KEY_Y); \
    X(KEY_Z); \
    X(KEY_SPACE); \
    X(KEY_ESCAPE); \
    X(KEY_ENTER); \
    X(KEY_TAB); \
    X(KEY_BACKSPACE); \
    X(KEY_INSERT); \
    X(KEY_DELETE); \
    X(KEY_RIGHT); \
    X(KEY_LEFT); \
    X(KEY_DOWN); \
    X(KEY_UP); \
    X(KEY_PAGE_UP); \
    X(KEY_PAGE_DOWN); \
    X(KEY_HOME); \
    X(KEY_END); \
    X(KEY_CAPS_LOCK); \
    X(KEY_SCROLL_LOCK); \
    X(KEY_NUM_LOCK); \
    X(KEY_PRINT_SCREEN); \
    X(KEY_PAUSE); \
    X(KEY_F1); \
    X(KEY_F2); \
    X(KEY_F3); \
    X(KEY_F4); \
    X(KEY_F5); \
    X(KEY_F6); \
    X(KEY_F7); \
    X(KEY_F8); \
    X(KEY_F9); \
    X(KEY_F10); \
    X(KEY_F11); \
    X(KEY_F12); \
    X(KEY_LEFT_SHIFT); \
    X(KEY_LEFT_CONTROL); \
    X(KEY_LEFT_ALT); \
    X(KEY_LEFT_SUPER); \
    X(KEY_RIGHT_SHIFT); \
    X(KEY_RIGHT_CONTROL); \
    X(KEY_RIGHT_ALT); \
    X(KEY_RIGHT_SUPER); \
    X(KEY_KB_MENU); \
    X(KEY_LEFT_BRACKET); \
    X(KEY_BACKSLASH); \
    X(KEY_RIGHT_BRACKET); \
    X(KEY_GRAVE); \
    X(KEY_KP_0); \
    X(KEY_KP_1); \
    X(KEY_KP_2); \
    X(KEY_KP_3); \
    X(KEY_KP_4); \
    X(KEY_KP_5); \
    X(KEY_KP_6); \
    X(KEY_KP_7); \
    X(KEY_KP_8); \
    X(KEY_KP_9); \
    X(KEY_KP_DECIMAL); \
    X(KEY_KP_DIVIDE); \
    X(KEY_KP_MULTIPLY); \
    X(KEY_KP_SUBTRACT); \
    X(KEY_KP_ADD); \
    X(KEY_KP_ENTER); \
    X(KEY_KP_EQUAL); \
    } while(0)

#define SET_KEY_DOWN(KEY) io.KeysDown[KEY] = IsKeyDown(KEY)


static void rlImGuiEvents()
{
	ImGuiIO& io = ImGui::GetIO();

	io.KeyCtrl = IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL);
	io.KeyShift = IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT);
	io.KeyAlt = IsKeyDown(KEY_RIGHT_ALT) || IsKeyDown(KEY_LEFT_ALT);
	io.KeySuper = IsKeyDown(KEY_RIGHT_SUPER) || IsKeyDown(KEY_LEFT_SUPER);

	for (size_t i = 0; i < ImGuiKey_KeysData_SIZE; i++)
		io.KeysData[i].Down = false;

	auto setDown = [&io](KeyboardKey raylibKey, ImGuiKey imGuiKey)
	{
		io.KeysData[ImGui::GetKeyIndex(imGuiKey)].Down = IsKeyDown(raylibKey);
	};

	setDown(KEY_APOSTROPHE, ImGuiKey_Apostrophe);
	setDown(KEY_COMMA, ImGuiKey_Comma);
	setDown(KEY_MINUS, ImGuiKey_Minus);
	setDown(KEY_PERIOD, ImGuiKey_Period);
	setDown(KEY_SLASH, ImGuiKey_Slash);
	setDown(KEY_ZERO, ImGuiKey_0);
	setDown(KEY_ONE, ImGuiKey_1);
	setDown(KEY_TWO, ImGuiKey_2);
	setDown(KEY_THREE, ImGuiKey_3);
	setDown(KEY_FOUR, ImGuiKey_4);
	setDown(KEY_FIVE, ImGuiKey_5);
	setDown(KEY_SIX, ImGuiKey_6);
	setDown(KEY_SEVEN, ImGuiKey_7);
	setDown(KEY_EIGHT, ImGuiKey_8);
	setDown(KEY_NINE, ImGuiKey_9);
	setDown(KEY_SEMICOLON, ImGuiKey_Semicolon);
	setDown(KEY_EQUAL, ImGuiKey_Equal);
	setDown(KEY_A, ImGuiKey_A);
	setDown(KEY_B, ImGuiKey_B);
	setDown(KEY_C, ImGuiKey_C);
	setDown(KEY_D, ImGuiKey_D);
	setDown(KEY_E, ImGuiKey_E);
	setDown(KEY_F, ImGuiKey_F);
	setDown(KEY_G, ImGuiKey_G);
	setDown(KEY_H, ImGuiKey_H);
	setDown(KEY_I, ImGuiKey_I);
	setDown(KEY_J, ImGuiKey_J);
	setDown(KEY_K, ImGuiKey_K);
	setDown(KEY_L, ImGuiKey_L);
	setDown(KEY_M, ImGuiKey_M);
	setDown(KEY_N, ImGuiKey_N);
	setDown(KEY_O, ImGuiKey_O);
	setDown(KEY_P, ImGuiKey_P);
	setDown(KEY_Q, ImGuiKey_Q);
	setDown(KEY_R, ImGuiKey_R);
	setDown(KEY_S, ImGuiKey_S);
	setDown(KEY_T, ImGuiKey_T);
	setDown(KEY_U, ImGuiKey_U);
	setDown(KEY_V, ImGuiKey_V);
	setDown(KEY_W, ImGuiKey_W);
	setDown(KEY_X, ImGuiKey_X);
	setDown(KEY_Y, ImGuiKey_Y);
	setDown(KEY_Z, ImGuiKey_Z);
	setDown(KEY_SPACE, ImGuiKey_Space);
	setDown(KEY_ESCAPE, ImGuiKey_Escape);
	setDown(KEY_ENTER, ImGuiKey_Enter);
	setDown(KEY_TAB, ImGuiKey_Tab);
	setDown(KEY_BACKSPACE, ImGuiKey_Backspace);
	setDown(KEY_INSERT, ImGuiKey_Insert);
	setDown(KEY_DELETE, ImGuiKey_Delete);
	setDown(KEY_RIGHT, ImGuiKey_RightArrow);
	setDown(KEY_LEFT, ImGuiKey_LeftArrow);
	setDown(KEY_DOWN, ImGuiKey_DownArrow);
	setDown(KEY_UP, ImGuiKey_UpArrow);
	setDown(KEY_PAGE_UP, ImGuiKey_PageUp);
	setDown(KEY_PAGE_DOWN, ImGuiKey_PageDown);
	setDown(KEY_HOME, ImGuiKey_Home);
	setDown(KEY_END, ImGuiKey_End);
	setDown(KEY_CAPS_LOCK, ImGuiKey_CapsLock);
	setDown(KEY_SCROLL_LOCK, ImGuiKey_ScrollLock);
	setDown(KEY_NUM_LOCK, ImGuiKey_NumLock);
	setDown(KEY_PRINT_SCREEN, ImGuiKey_PrintScreen);
	setDown(KEY_PAUSE, ImGuiKey_Pause);
	setDown(KEY_F1, ImGuiKey_F1);
	setDown(KEY_F2, ImGuiKey_F2);
	setDown(KEY_F3, ImGuiKey_F3);
	setDown(KEY_F4, ImGuiKey_F4);
	setDown(KEY_F5, ImGuiKey_F5);
	setDown(KEY_F6, ImGuiKey_F6);
	setDown(KEY_F7, ImGuiKey_F7);
	setDown(KEY_F8, ImGuiKey_F8);
	setDown(KEY_F9, ImGuiKey_F9);
	setDown(KEY_F10, ImGuiKey_F10);
	setDown(KEY_F11, ImGuiKey_F11);
	setDown(KEY_F12, ImGuiKey_F12);
	setDown(KEY_LEFT_SHIFT, ImGuiKey_LeftShift);
	setDown(KEY_LEFT_CONTROL, ImGuiKey_LeftCtrl);
	setDown(KEY_LEFT_ALT, ImGuiKey_LeftAlt);
	setDown(KEY_LEFT_SUPER, ImGuiKey_LeftSuper);
	setDown(KEY_RIGHT_SHIFT, ImGuiKey_RightShift);
	setDown(KEY_RIGHT_CONTROL, ImGuiKey_RightCtrl);
	setDown(KEY_RIGHT_ALT, ImGuiKey_RightAlt);
	setDown(KEY_RIGHT_SUPER, ImGuiKey_RightSuper);
	setDown(KEY_KB_MENU, ImGuiKey_Menu);
	setDown(KEY_LEFT_BRACKET, ImGuiKey_LeftBracket);
	setDown(KEY_BACKSLASH, ImGuiKey_Backslash);
	setDown(KEY_RIGHT_BRACKET, ImGuiKey_RightBracket);
	setDown(KEY_GRAVE, ImGuiKey_GraveAccent);
	setDown(KEY_KP_0, ImGuiKey_Keypad0);
	setDown(KEY_KP_1, ImGuiKey_Keypad1);
	setDown(KEY_KP_2, ImGuiKey_Keypad2);
	setDown(KEY_KP_3, ImGuiKey_Keypad3);
	setDown(KEY_KP_4, ImGuiKey_Keypad4);
	setDown(KEY_KP_5, ImGuiKey_Keypad5);
	setDown(KEY_KP_6, ImGuiKey_Keypad6);
	setDown(KEY_KP_7, ImGuiKey_Keypad7);
	setDown(KEY_KP_8, ImGuiKey_Keypad8);
	setDown(KEY_KP_9, ImGuiKey_Keypad9);
	setDown(KEY_KP_DECIMAL, ImGuiKey_KeypadDecimal);
	setDown(KEY_KP_DIVIDE, ImGuiKey_KeypadDivide);
	setDown(KEY_KP_MULTIPLY, ImGuiKey_KeypadMultiply);
	setDown(KEY_KP_SUBTRACT, ImGuiKey_KeypadSubtract);
	setDown(KEY_KP_ADD, ImGuiKey_KeypadAdd);
	setDown(KEY_KP_ENTER, ImGuiKey_KeypadEnter);
	setDown(KEY_KP_EQUAL, ImGuiKey_KeypadEqual);

	unsigned int pressed = GetCharPressed();
	if (pressed != 0)
		io.AddInputCharacter(pressed);
}

static void rlImGuiTriangleVert(ImDrawVert& idx_vert)
{
	Color* c;
	c = (Color*)&idx_vert.col;
	rlColor4ub(c->r, c->g, c->b, c->a);
	rlTexCoord2f(idx_vert.uv.x, idx_vert.uv.y);
	rlVertex2f(idx_vert.pos.x, idx_vert.pos.y);
}

static void rlImGuiRenderTriangles(unsigned int count, int indexStart, const ImVector<ImDrawIdx>& indexBuffer, const ImVector<ImDrawVert>& vertBuffer, void* texturePtr)
{
	if (count < 3)
		return;

	Texture* texture = (Texture*)texturePtr;

	unsigned int textureId = (texture == nullptr) ? 0 : texture->id;

	rlBegin(RL_TRIANGLES);
	rlSetTexture(textureId);

	for (unsigned int i = 0; i <= (count - 3); i += 3)
	{
		if (rlCheckRenderBatchLimit(3))
		{
			rlBegin(RL_TRIANGLES);
			rlSetTexture(textureId);
		}

		ImDrawIdx indexA = indexBuffer[indexStart + i];
		ImDrawIdx indexB = indexBuffer[indexStart + i + 1];
		ImDrawIdx indexC = indexBuffer[indexStart + i + 2];

		ImDrawVert vertexA = vertBuffer[indexA];
		ImDrawVert vertexB = vertBuffer[indexB];
		ImDrawVert vertexC = vertBuffer[indexC];

		rlImGuiTriangleVert(vertexA);
		rlImGuiTriangleVert(vertexB);
		rlImGuiTriangleVert(vertexC);
	}
	rlEnd();
}

static void EnableScissor(float x, float y, float width, float height)
{
	rlEnableScissorTest();
	ImGuiIO& io = ImGui::GetIO();
	rlScissor((int)(x * io.DisplayFramebufferScale.x),
		int((GetScreenHeight() - (int)(y + height)) * io.DisplayFramebufferScale.y),
		(int)(width * io.DisplayFramebufferScale.x),
		(int)(height * io.DisplayFramebufferScale.y));
}

static void rlRenderData(ImDrawData* data)
{
	rlDrawRenderBatchActive();
	rlDisableBackfaceCulling();

	for (int l = 0; l < data->CmdListsCount; ++l)
	{
		const ImDrawList* commandList = data->CmdLists[l];

		for (const auto& cmd : commandList->CmdBuffer)
		{
			EnableScissor(cmd.ClipRect.x - data->DisplayPos.x, cmd.ClipRect.y - data->DisplayPos.y, cmd.ClipRect.z - (cmd.ClipRect.x - data->DisplayPos.x), cmd.ClipRect.w - (cmd.ClipRect.y - data->DisplayPos.y));
			if (cmd.UserCallback != nullptr)
			{
				cmd.UserCallback(commandList, &cmd);

				continue;
			}

			rlImGuiRenderTriangles(cmd.ElemCount, cmd.IdxOffset, commandList->IdxBuffer, commandList->VtxBuffer, cmd.TextureId);
			rlDrawRenderBatchActive();
		}
	}

	rlSetTexture(0);
	rlDisableScissorTest();
	rlEnableBackfaceCulling();
}

void SetupMouseCursors()
{
	MouseCursorMap[ImGuiMouseCursor_Arrow] = MOUSE_CURSOR_ARROW;
	MouseCursorMap[ImGuiMouseCursor_TextInput] = MOUSE_CURSOR_IBEAM;
	MouseCursorMap[ImGuiMouseCursor_Hand] = MOUSE_CURSOR_POINTING_HAND;
	MouseCursorMap[ImGuiMouseCursor_ResizeAll] = MOUSE_CURSOR_RESIZE_ALL;
	MouseCursorMap[ImGuiMouseCursor_ResizeEW] = MOUSE_CURSOR_RESIZE_EW;
	MouseCursorMap[ImGuiMouseCursor_ResizeNESW] = MOUSE_CURSOR_RESIZE_NESW;
	MouseCursorMap[ImGuiMouseCursor_ResizeNS] = MOUSE_CURSOR_RESIZE_NS;
	MouseCursorMap[ImGuiMouseCursor_ResizeNWSE] = MOUSE_CURSOR_RESIZE_NWSE;
	MouseCursorMap[ImGuiMouseCursor_NotAllowed] = MOUSE_CURSOR_NOT_ALLOWED;
}

void rlImGuiBeginInitImGui()
{
	ImGui::CreateContext(nullptr);
}

void rlImGuiEndInitImGui()
{
	SetupMouseCursors();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendPlatformName = "imgui_impl_raylib";

	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

	io.MousePos = ImVec2(0, 0);

	io.SetClipboardTextFn = rlImGuiSetClipText;
	io.GetClipboardTextFn = rlImGuiGetClipText;

	io.ClipboardUserData = nullptr;

	rlImGuiReloadFonts();
}

void rlImGuiSetup(bool dark)
{
	rlImGuiBeginInitImGui();

	if (dark)
		ImGui::StyleColorsDark();
	else
		ImGui::StyleColorsLight();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();

#ifndef NO_FONT_AWESOME
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.FontDataOwnedByAtlas = false;
	io.Fonts->AddFontFromMemoryCompressedTTF((void*)fa_solid_900_compressed_data, fa_solid_900_compressed_size, FONT_AWESOME_ICON_SIZE, &icons_config, icons_ranges);
#endif

	rlImGuiEndInitImGui();
}

void rlImGuiReloadFonts()
{
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels = nullptr;

	int width;
	int height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, nullptr);
	Image image = GenImageColor(width, height, BLANK);
	memcpy(image.data, pixels, width * height * 4);

	if (FontTexture.id != 0)
		UnloadTexture(FontTexture);

	FontTexture = LoadTextureFromImage(image);
	UnloadImage(image);
	io.Fonts->TexID = &FontTexture;
}

void rlImGuiBegin()
{
	rlImGuiNewFrame();
	rlImGuiEvents();
	ImGui::NewFrame();
}

void rlImGuiEnd()
{
	ImGui::Render();
	rlRenderData(ImGui::GetDrawData());
}

void rlImGuiShutdown()
{
	UnloadTexture(FontTexture);

	ImGui::DestroyContext();
}

void rlImGuiImage(const Texture* image)
{
	ImGui::Image((ImTextureID)image, ImVec2(float(image->width), float(image->height)));
}

bool rlImGuiImageButton(const char* name, const Texture* image)
{
	return ImGui::ImageButton(name, (ImTextureID)image, ImVec2(float(image->width), float(image->height)));
}

bool rlImGuiImageButtonSize(const char* name, const Texture* image, ImVec2 size)
{
	return ImGui::ImageButton(name, (ImTextureID)image, size);
}

void rlImGuiImageSize(const Texture* image, int width, int height)
{
	ImGui::Image((ImTextureID)image, ImVec2(float(width), float(height)));
}

void rlImGuiImageRect(const Texture* image, int destWidth, int destHeight, Rectangle sourceRect)
{
	ImVec2 uv0;
	ImVec2 uv1;

	if (sourceRect.width < 0)
	{
		uv0.x = -((float)sourceRect.x / image->width);
		uv1.x = (uv0.x - (float)(fabs(sourceRect.width) / image->width));
	}
	else
	{
		uv0.x = (float)sourceRect.x / image->width;
		uv1.x = uv0.x + (float)(sourceRect.width / image->width);
	}

	if (sourceRect.height < 0)
	{
		uv0.y = -((float)sourceRect.y / image->height);
		uv1.y = (uv0.y - (float)(fabs(sourceRect.height) / image->height));
	}
	else
	{
		uv0.y = (float)sourceRect.y / image->height;
		uv1.y = uv0.y + (float)(sourceRect.height / image->height);
	}

	ImGui::Image((ImTextureID)image, ImVec2(float(destWidth), float(destHeight)), uv0, uv1);
}
