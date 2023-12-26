#include "layout.h"
#include "layout_common.h"
#include "app/window.h"
#include "app/app.h"

namespace MiniG::Gui
{
static BUTTON_CALLBACK_FUNC(SetWindowShouldClose)
{
	MINIG_UNUSED(args);
	glfwSetWindowShouldClose(MainApp::GetWindow(), GLFW_TRUE);
}

static Button MM_SinglePlayer_Button = {
	.Label = "Play Games",
	.Size  = {400, 50},
	.Position = {600, 250},
	.pButtonPressedCallback = LayoutManager::SwitchLayout,
	.CallbackArgs = { LayoutName_SelectGame },
};

static Button MM_Settings_Button = {
	.Label = "Settings",
	.Size  = {400, 50},
	.Position = {600, 400},
	.pButtonPressedCallback = LayoutManager::SwitchLayout,
	.CallbackArgs = { "Settings" },
};

static Button MM_Exit_Button = {
	.Label = "Exit",
	.Size  = {400, 50},
	.Position = {600, 475},
	.pButtonPressedCallback = SetWindowShouldClose,
	.CallbackArgs = {},
};

static LayoutWindow MM_Buttons_Window = {
	.Label     = "Buttons",
	.Size      = WINDOW_SIZE_FULLSCREEN,
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {
		{ ItemType::Button, &MM_SinglePlayer_Button },
		{ ItemType::Button, &MM_Settings_Button },
		{ ItemType::Button, &MM_Exit_Button },
	},
};

static LayoutWindow MM_Background_Window = {
	.Label     = "Background",
	.Size      = WINDOW_SIZE_FULLSCREEN,
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {},
};

Layout Layout_MainMenu = {
	.Name = LayoutName_MainMenu,
	.WindowStack = {
		&MM_Background_Window, &MM_Buttons_Window
	},
};

void Init_LayoutMainMenu()
{

}
} /* namespace MiniG::Gui */