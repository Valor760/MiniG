#pragma once
#include "game.h"
#include "app/common.h"
#include "app/resources/texture.h"

#include <array>
#include <map>

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

enum class TetraminoShape
{
	Shape_I, Shape_J, Shape_L,
	Shape_O, Shape_S, Shape_T,
	Shape_Z,

	Shape_Count
};

struct Block
{
	bool IsSet = false;
	BlockColor Color = BlockColor::ElementCount;
};

/* TODO: How to turn blocks around? */
struct Tetramino
{
	TetraminoShape Shape = TetraminoShape::Shape_Count;
	std::vector<MGVec2<int>> OccupiedCells = {};
	BlockColor Color = BlockColor::ElementCount;
};

enum MoveAction
{
	ToLeft = -1,
	ToRight = 1,
};

/*
	Possible figures(tetraminos):
	                          
	[]                        
	[]      []    []      [][]
	[]      []    []      [][]
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
		void ProcessInput();

		std::vector<MGVec2<int>> getDownMostCoords();
		void applyTetraminoToField();
		void TimeMoveFallingTetramino();

		void MoveFallingTetraminoToSide(int where);
		void DropFallingTetramino();

	private:
		double m_PassedTime = 0.0;

		/* Tetris field is 10 x 20 */
		std::array<std::array<Block, 10>, 20> m_Field = {};

		std::map<BlockTexture, Resources::Texture> m_BlockTextures;

		std::shared_ptr<Tetramino> m_FallingTetramino = nullptr;
		std::shared_ptr<Tetramino> m_NextTetramino = nullptr;
};
} /* namespace MiniG::Games */