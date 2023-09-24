#pragma once
#include "game.h"
#include "app/common.h"
#include "app/resources/texture.h"

#include <array>

namespace MiniG::Games
{

class TicTacToe: public Game
{
	public:
		TicTacToe() : Game() {}

		virtual void OnAttach();
		virtual void OnUpdate(double dt);
		virtual void OnDetach();
	
	public:
		const std::string m_GameName = "TicTacToe";

	
};
} /* namespace MiniG::Games */