#pragma once
#include "app/common.h"

namespace MiniG::Games
{
class Game
{
	public:
		virtual void OnAttach();
		virtual void OnUpdate();
		virtual void OnDetach();

	private:
		std::string m_GameName = "";
};
}