#pragma once
#include "app/common.h"

namespace MiniG::Games
{
class Game
{
	public:
		virtual void OnAttach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDetach() = 0;

	public:
		const std::string m_GameName = "";
};
} /* namespace MiniG::Games */