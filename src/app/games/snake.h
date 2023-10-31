#pragma once
#include "game.h"
#include "app/common.h"

namespace MiniG::Games
{
class Snake : public Game
{
	public:
		Snake() : Game() {}

		virtual void OnAttach();
		virtual void OnUpdate(double dt);
		virtual void OnDetach();

	public:
		const std::string m_GameName = "Snake";

	private:
		
};
} /* namespace MiniG::Games */