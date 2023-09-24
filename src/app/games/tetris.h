#pragma once
#include "game.h"
#include "app/common.h"
#include "app/resources/texture.h"

#include <array>

namespace MiniG::Games
{
enum class BlockColor
{
	ColorRed,
	ColorGreen,
	ColorYellow,
	ColorBlue,
	ColorPurple,

	ElementCount
};

enum class BlockTexture
{
	Wall,
	Block,

	ElementCount
};

struct Block
{
	BlockColor Color = BlockColor::ElementCount;
	bool IsSet = false;
};

struct Tetramino
{
	std::vector<MGVec2<int>> OccupiedCells = {};
};

/*
	Possible figures(tetraminos):
	                          
	[]      []    []      [][]
	[]      []    []      [][]
	[]      []    []          
	[]    [][]    [][]        
	                          
	  [][]    [][][]    [][]  
	[][]        []        [][]
	                          
	Each of them can be rotated 90 degrees
*/
class Tetris : public Game
{
	public:
		Tetris() : Game() {}

		virtual void OnAttach();
		virtual void OnUpdate(double dt);
		virtual void OnDetach();
	
	public:
		const std::string m_GameName = "Tetris";

	private:
		void drawField();
		void drawScoreBoard();

	private:
		double m_PassedTime = 0.0;

		/* Tetris field is 10 x 20, each cell will be s */
		std::array<std::array<Block, 10>, 20> m_Field = {};

		Tetramino m_FallingTetramino;
};
} /* namespace MiniG::Games */