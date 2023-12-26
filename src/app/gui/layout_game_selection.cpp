#include "layout_common.h"
#include "app/app.h"

#include <map>

namespace MiniG::Gui
{
extern std::map<std::string, Games::Game*> g_GamesMap;

/* Provide parent layout name as the first argument */
static BUTTON_CALLBACK_FUNC(GameExit)
{
	/* Unload current game */
	MainApp::LoadGame("");

	if(args.size() != 1)
	{
		LOG_ERROR("Wrong number of arguments! provided=%d expected=%d", args.size(), 1);
		return;
	}

	Gui::LayoutManager::SwitchLayout(args);
}

static Button Game_Back_Button = {
	.Label = "Main Menu",
	.Size = {150, 50},
	.Position = {20, 20},
	.pButtonPressedCallback = GameExit,
	.CallbackArgs = {LayoutName_SelectGame},
};

static LayoutWindow Tetris_Background_Window = {
	.Label      = "Background",
	.Size       = WINDOW_SIZE_FULLSCREEN,
	.Position   = POSITION_DEFAULT,
	.Flags      = WINDOW_BACKGROUND_FLAGS,
	.Items      = {
		{ ItemType::Button, &Game_Back_Button },
	},
	.Background = Resources::Texture(),
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
	.Label = "Coming Soon",
	.Size = {366, 100},
	.Position = {616, 425},
	.pButtonPressedCallback = nullptr,
	.CallbackArgs = {}
};

static Button SG_TODO_Button = {
	.Label = "Coming Soon",
	.Size = {366, 100},
	.Position = {1032, 425},
	.pButtonPressedCallback = nullptr,
	.CallbackArgs = {}
};

static Button SG_Back_Button = {
	.Label = "Back",
	.Size = {150, 50},
	.Position = {20, 20},
	.pButtonPressedCallback = GameExit,
	.CallbackArgs = {LayoutName_MainMenu},
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
		{ ItemType::Button, &SG_Back_Button },
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

void Init_LayoutGameSelection()
{
	Tetris_Background_Window.Background = Resources::Texture("assets/tetris-game-bg.png");
}
} /* namespace MiniG::Gui */