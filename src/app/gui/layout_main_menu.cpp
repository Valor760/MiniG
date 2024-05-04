#include "layout.h"
#include "layout_common.h"
#include "app/window.h"

namespace MiniG::Gui
{
static BUTTON_CALLBACK_FUNC(SetWindowShouldClose)
{
	MINIG_UNUSED(args);
	// glfwSetWindowShouldClose(Window::GetWindow(), GLFW_TRUE);
}

static Button MM_SinglePlayer_Button = {
	.Label = "Play Games",
	.Size  = {600, 100},
	.Position = {500, 250},
	.pButtonPressedCallback = LayoutManager::SwitchLayout,
	.CallbackArgs = { LayoutName_SelectGame },
	.IsShadow = false,
};

static Button MM_SinglePlayerShadow_Button = {
	.Label = "Play Games",
	.Size  = {600, 100},
	.Position = {505, 255},
	.pButtonPressedCallback = LayoutManager::SwitchLayout,
	.CallbackArgs = { LayoutName_SelectGame },
	.IsShadow = true,
};

static Button MM_ExitShadow_Button = {
	.Label = "Exit",
	.Size  = {500, 100},
	.Position = {555, 480},
	.pButtonPressedCallback = SetWindowShouldClose,
	.CallbackArgs = {},
	.IsShadow = true,
};

static Button MM_Exit_Button = {
	.Label = "Exit",
	.Size  = {500, 100},
	.Position = {550, 475},
	.pButtonPressedCallback = SetWindowShouldClose,
	.CallbackArgs = {},
	.IsShadow = false,
};

static LayoutWindow MM_Buttons_Window = {
	.Label     = "Buttons",
	.Size      = WINDOW_SIZE_FULLSCREEN,
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {
		{ ItemType::Button, &MM_SinglePlayerShadow_Button },
		{ ItemType::Button, &MM_SinglePlayer_Button },
		{ ItemType::Button, &MM_ExitShadow_Button },
		{ ItemType::Button, &MM_Exit_Button },
	},
	.Style = {
		{ .Var = ImGuiStyleVar_FrameRounding, .Type = StyleVarType::Float, .Value = 10.0f },
	},
	.Color = {
		{ .Type = ImGuiCol_Button,        .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
		{ .Type = ImGuiCol_ButtonHovered, .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
		{ .Type = ImGuiCol_ButtonActive,  .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
	},
	.Font = nullptr,
};

static LayoutWindow MM_Background_Window = {
	.Label     = "Background",
	.Size      = WINDOW_SIZE_FULLSCREEN,
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {},
};

MAKE_LAYOUT(MainMenu) = {
	.Name = LayoutName_MainMenu,
	.WindowStack = {
		&MM_Background_Window, &MM_Buttons_Window
	},
};

void Init_LayoutMainMenu()
{
	LOG_DEBUG("");
	ImGuiIO& io = ImGui::GetIO();
	MM_Buttons_Window.Font = io.Fonts->AddFontFromFileTTF("assets/Score-Font.ttf", 75.0f);
	assert(MM_Buttons_Window.Font);
}
} /* namespace MiniG::Gui */