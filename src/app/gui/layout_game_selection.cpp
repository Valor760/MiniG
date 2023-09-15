#include "layout_common.h"

namespace MiniG::Gui
{
static Button SG_Tetris_Button = {
	.Label = "Tetris",
	.Size = {366, 100},
	.Position = {200, 425},
	.pButtonPressedCallback = nullptr,
	.CallbackArgs = {}
};

static Button SG_Snake_Button = {
	.Label = "Snake",
	.Size = {366, 100},
	.Position = {616, 425},
	.pButtonPressedCallback = nullptr,
	.CallbackArgs = {}
};

static Button SG_TODO_Button = {
	.Label = "TODO",
	.Size = {366, 100},
	.Position = {1032, 425},
	.pButtonPressedCallback = nullptr,
	.CallbackArgs = {}
};

static LayoutWindow MM_Buttons_Window = {
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

static LayoutWindow MM_Background_Window = {
	.Label     = "Background",
	.Size      = WINDOW_SIZE_FULLSCREEN,
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {},
};

Layout Layout_SelectGame = {
	.Name = LayoutName_SelectGame,
	.WindowStack = {
		&MM_Background_Window, &MM_Buttons_Window
	},
};
} /* namespace MiniG::Gui */