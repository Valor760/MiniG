#include "layout_common.h"
#include "app/app.h"

namespace MiniG::Gui
{
static LayoutWindow Tetris_Background_Window = {
	.Label     = "Background",
	.Size      = WINDOW_SIZE_FULLSCREEN,
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {},
};

Layout Layout_Tetris = {
	.Name = LayoutName_Tetris,
	.WindowStack = {
		&Tetris_Background_Window /* Other windows will be manually added in Tetris class */
	},
};

static BUTTON_CALLBACK_FUNC(SelectTetris)
{
	MINIG_UNUSED(args);

	/* TODO: Need to load some sort of background layout, like background image, buttons and etc. */
	LayoutManager::SwitchLayout({LayoutName_Tetris});
	MainApp::LoadGame("Tetris");
}

static Button SG_Tetris_Button = {
	.Label = "Tetris",
	.Size = {366, 100},
	.Position = {200, 425},
	.pButtonPressedCallback = SelectTetris,
	.CallbackArgs = {}
};

static Button SG_Snake_Button = {
	.Label = "Snake",
	.Size = {366, 100},
	.Position = {616, 425},
	.pButtonPressedCallback = nullptr,
	.CallbackArgs = {}
};


static BUTTON_CALLBACK_FUNC(SelectTicTacToe)
{
	MINIG_UNUSED(args);

	/* TODO: Need to load some sort of background layout, like background image, buttons and etc. */
	LayoutManager::SwitchLayout({LayoutName_Tetris});
	
	MainApp::LoadGame("TicTacToe");
}

static Button SG_TODO_Button = {
	.Label = "TODO",
	.Size = {366, 100},
	.Position = {1032, 425},
	.pButtonPressedCallback = SelectTicTacToe,
	.CallbackArgs = {}
};

static LayoutWindow SG_Buttons_Window = {
	.Label     = "Buttons",
	.Size      = WINDOW_SIZE_FULLSCREEN,
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {
		{ ItemType::Button, &SG_Tetris_Button },
		{ ItemType::Button, &SG_Snake_Button },
		{ ItemType::Button, &SG_TODO_Button },
	},
};

static LayoutWindow SG_Background_Window = {
	.Label     = "Background",
	.Size      = WINDOW_SIZE_FULLSCREEN,
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {},
};

Layout Layout_SelectGame = {
	.Name = LayoutName_SelectGame,
	.WindowStack = {
		&SG_Background_Window, &SG_Buttons_Window
	},
};
} /* namespace MiniG::Gui */