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
	enum class GameState
	{
		Start, InProgress, GameOver,
	};

	public:
		Snake() : Game() {}

		virtual void OnAttach();
		virtual void OnUpdate(double dt);
		virtual void OnDetach();

		void LoadFont();

	public:
		const std::string m_GameName = "Snake";

	private:
		void drawField();
		void processMovement();
		void processInput();
		void tryApplyDirection(Direction dir);
		void generateFruitNewPos();
		bool steppedOnTail(const MGVec2<int>& head_coord);

	private:
		double m_PassedTime;
		bool m_HasDirectionChanged;
		bool m_ShouldAddBody;

		std::array<std::array<CellType, 60>, 35> m_Field;
		std::vector<MGVec2<int>> m_SnakeBodyCells;
		MGVec2<int> m_HeadPos;
		MGVec2<int> m_FruitPos;
		Direction m_MovementDirection;

		std::map<CellType, Resources::Texture> m_Textures;

		GameState m_GameState = GameState::Start;

		ImFont* m_Font;
		ImVec2 m_GameStartTextSize;
		ImVec2 m_GameOverTextSize;
};
} /* namespace MiniG::Games */