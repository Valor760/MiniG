#pragma once
#include "common.h"
#include "window.h"
#include "games/game.h"

namespace MiniG
{
class MainApp
{
	public:
		MainApp() {}
		~MainApp();

		bool Init();
		void Run();

	private:
		/* For now we don't need to have more than 1 window */
		Window m_Window;
		Games::Game* m_CurrentGame = nullptr;
};
} /* namespace MiniG */