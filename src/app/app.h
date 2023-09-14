#pragma once
#include "common.h"
#include "window.h"

namespace MiniG
{
class MainApp
{
	public:
		MainApp() : m_Window()
		{}
		~MainApp();

		bool Init();
		void Run();

	private:
		/* For now we don't need to have more than 1 window */
		Window m_Window;
};
} /* namespace MiniG */