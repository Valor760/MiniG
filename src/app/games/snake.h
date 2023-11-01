#pragma once
#include "game.h"
#include "app/common.h"
#include "app/resources/texture.h"

#include <array>
#include <map>
#include <vector>

namespace MiniG::Games
{
enum class CellType
{
	Empty, Head, Body, Fruit,

	Count
};

enum class Direction
{
	Up, Down, Left, Right
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
		void processMovement();

	private:
		double m_PassedTime;

		std::array<std::array<CellType, 60>, 35> m_Field;
		std::vector<MGVec2<int>> m_SnakeBodyCells;
		MGVec2<int> m_HeadPos;
		MGVec2<int> m_FruitPos;
		Direction m_MovementDirection;

		std::map<CellType, Resources::Texture> m_Textures;
};
} /* namespace MiniG::Games */