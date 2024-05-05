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
	.Label = "Main\nMenu",
	.Size = {175, 100},
	.Position = {20, 15},
	.pButtonPressedCallback = GameExit,
	.CallbackArgs = {LayoutName_SelectGame},
	.IsShadow = false,
};

static Button Game_BackShadow_Button = {
	.Label = "Main\nMenu",
	.Size = {175, 100},
	.Position = {25, 20},
	.pButtonPressedCallback = GameExit,
	.CallbackArgs = {LayoutName_SelectGame},
	.IsShadow = true,
};

/* --- Snake --- */
static LayoutWindow Snake_Background_Window = {
	.Label      = "Background",
	.Size       = WINDOW_SIZE_FULLSCREEN,
	.Position   = POSITION_DEFAULT,
	.Flags      = WINDOW_BACKGROUND_FLAGS,
	.Items      = {
		{ ItemType::Button, &Game_BackShadow_Button },
		{ ItemType::Button, &Game_Back_Button },
	},
	.Background = Resources::Texture(),
	.Style = {},
	.Color = {
		{ .Type = ImGuiCol_Button,        .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
		{ .Type = ImGuiCol_ButtonHovered, .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
		{ .Type = ImGuiCol_ButtonActive,  .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
	},
	.Font = nullptr,
};

MAKE_LAYOUT(Snake) = {
	.Name = LayoutName_Snake,
	.WindowStack = {
		&Snake_Background_Window /* Other windows will be manually added in Tetris class */
	},
};

static BUTTON_CALLBACK_FUNC(SelectSnake)
{
	MINIG_UNUSED(args);

	LayoutManager::SwitchLayout({LayoutName_Snake});
	MainApp::LoadGame("Snake");
}
/* --- Snake --- */


/* --- Tetris --- */
static LayoutWindow Tetris_Background_Window = {
	.Label      = "Background",
	.Size       = WINDOW_SIZE_FULLSCREEN,
	.Position   = POSITION_DEFAULT,
	.Flags      = WINDOW_BACKGROUND_FLAGS,
	.Items      = {
		{ ItemType::Button, &Game_BackShadow_Button },
		{ ItemType::Button, &Game_Back_Button },
	},
	.Background = Resources::Texture(),
	.Style = {},
	.Color = {
		{ .Type = ImGuiCol_Button,        .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
		{ .Type = ImGuiCol_ButtonHovered, .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
		{ .Type = ImGuiCol_ButtonActive,  .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
	},
	.Font = nullptr,
};

MAKE_LAYOUT(Tetris) = {
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
/* --- Tetris --- */


static Button SG_Tetris_Button = {
	.Label = "Tetris",
	.Size = {400, 100},
	.Position = {200, 425},
	.pButtonPressedCallback = SelectTetris,
	.CallbackArgs = {},
	.IsShadow = false,
};

static Button SG_TetrisShadow_Button = {
	.Label = "Tetris",
	.Size = {400, 100},
	.Position = {205, 430},
	.pButtonPressedCallback = SelectTetris,
	.CallbackArgs = {},
	.IsShadow = true,
};

static Button SG_Snake_Button = {
	.Label = "Snake",
	.Size = {400, 100},
	.Position = {1000, 425},
	.pButtonPressedCallback = SelectSnake,
	.CallbackArgs = {},
	.IsShadow = false,
};

static Button SG_SnakeShadow_Button = {
	.Label = "Snake",
	.Size = {400, 100},
	.Position = {1005, 430},
	.pButtonPressedCallback = SelectSnake,
	.CallbackArgs = {},
	.IsShadow = true,
};

static Button SG_Back_Button = {
	.Label = "Back",
	.Size = {185, 75},
	.Position = {20, 15},
	.pButtonPressedCallback = GameExit,
	.CallbackArgs = {LayoutName_MainMenu},
	.IsShadow = false,
};

static Button SG_BackShadow_Button = {
	.Label = "Back",
	.Size = {185, 75},
	.Position = {25, 20},
	.pButtonPressedCallback = GameExit,
	.CallbackArgs = {LayoutName_MainMenu},
	.IsShadow = true,
};

static LayoutWindow SG_BackButton_Window = {
	.Label     = "BackButton",
	.Size      = ImVec2(300, 200),
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {
		{ ItemType::Button, &SG_BackShadow_Button },
		{ ItemType::Button, &SG_Back_Button },
	},
	.Style = {},
	.Color = {
		{ .Type = ImGuiCol_Button,        .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
		{ .Type = ImGuiCol_ButtonHovered, .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
		{ .Type = ImGuiCol_ButtonActive,  .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
	},
	.Font = nullptr,
};

static LayoutWindow SG_Buttons_Window = {
	.Label     = "Buttons",
	.Size      = WINDOW_SIZE_FULLSCREEN,
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {
		{ ItemType::Button, &SG_TetrisShadow_Button },
		{ ItemType::Button, &SG_Tetris_Button },
		{ ItemType::Button, &SG_SnakeShadow_Button },
		{ ItemType::Button, &SG_Snake_Button },
	},
	.Style = {},
	.Color = {
		{ .Type = ImGuiCol_Button,        .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
		{ .Type = ImGuiCol_ButtonHovered, .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
		{ .Type = ImGuiCol_ButtonActive,  .Color = ImVec4(0.f, 0.f, 0.f, 0.f) },
	},
	.Font = nullptr,
};

static LayoutWindow SG_Background_Window = {
	.Label     = "Background",
	.Size      = WINDOW_SIZE_FULLSCREEN,
	.Position  = POSITION_DEFAULT,
	.Flags     = WINDOW_BACKGROUND_FLAGS,
	.Items     = {},
};

MAKE_LAYOUT(SelectGame) = {
	.Name = LayoutName_SelectGame,
	.WindowStack = {
		&SG_Background_Window, &SG_Buttons_Window, &SG_BackButton_Window
	},
};

void Init_LayoutGameSelection()
{
	LOG_DEBUG("");

	Tetris_Background_Window.Background = Resources::Texture("assets/tetris-game-bg.png");
	Snake_Background_Window.Background  = Resources::Texture("assets/snake-bg.jpg");
	SG_Background_Window.Background     = Resources::Texture("assets/game-select-bg.png");

	ImGuiIO& io = ImGui::GetIO();
	SG_Buttons_Window.Font = io.Fonts->AddFontFromFileTTF("assets/Score-Font.ttf", 75.0f);
	assert(SG_Buttons_Window.Font);

	SG_BackButton_Window.Font = io.Fonts->AddFontFromFileTTF("assets/Score-Font.ttf", 50.0f);
	assert(SG_BackButton_Window.Font);
	Tetris_Background_Window.Font = SG_BackButton_Window.Font;
	Snake_Background_Window.Font  = SG_BackButton_Window.Font;
}
} /* namespace MiniG::Gui */