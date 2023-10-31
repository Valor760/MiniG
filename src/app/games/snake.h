#pragma once
#include "game.h"
#include "app/common.h"
#include "app/resources/texture.h"

#include <array>
#include <map>

namespace MiniG::Games
{
enum class CellType
{
	Empty, Head, Body, Fruit,

	Count
};

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
		void drawField();

	private:
		double m_PassedTime;

		std::array<std::array<CellType, 60>, 35> m_Field;
		MGVec2<int> m_HeadPos;
		MGVec2<int> m_FruitPos;

		std::map<CellType, Resources::Texture> m_Textures;
};
} /* namespace MiniG::Games */