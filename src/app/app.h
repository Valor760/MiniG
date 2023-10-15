#pragma once
#include "common.h"
#include "window.h"
#include "games/game.h"

namespace MiniG
{
/* App is only one, so it is a static class */
class MainApp
{
	public:
		/* Init basic application settings */
		static bool Init();
		/* Main application loop */
		static void Run();

		/* Load a desired game. Empty string will unload the game if it was previosly loaded */
		static void LoadGame(const std::string& game_name);

	private:
		MainApp() = delete;
		~MainApp() = delete;

	private:
		/* For now we don't need to have more than 1 window */
		static inline Window m_Window;
		static inline Games::Game* m_CurrentGame = nullptr;
};
} /* namespace MiniG */