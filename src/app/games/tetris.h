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

enum class Texture
{
	Wall,
	Block,
	FieldBG,
	BlockProjection,

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

struct Tetramino
{
	TetraminoShape Shape = TetraminoShape::Shape_Count;
	std::vector<MGVec2<int>> OccupiedCells = {};
	BlockColor Color = BlockColor::ElementCount;
	int RotationState = 0;
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

		/* Workaround, because font can't be loaded in OnAttach */
		void LoadFont();
	
	public:
		const std::string m_GameName = "Tetris";

	private:
		/* TODO: Refactor names to start from lowercase for private functions */
		void drawField();
		void drawScoreBoard();
		void ProcessInput();

		void applyTetraminoToField();
		void TimeMoveFallingTetramino();

		void MoveFallingTetraminoToSide(int where);
		void DropFallingTetramino();
		void rorateFallingTetramino();

		void checkAndRemoveLines();

	private:
		double m_PassedTime = 0.0;
		int m_Score = 0;

		/* Tetris field is 10 x 20 */
		std::array<std::array<Block, 10>, 20> m_Field = {};

		std::map<Texture, Resources::Texture> m_Textures;

		std::shared_ptr<Tetramino> m_FallingTetramino = nullptr;
		std::shared_ptr<Tetramino> m_NextTetramino = nullptr;

		ImFont* m_ScoreBoardFont = nullptr;
		ImVec2 m_ScoreTextSize = {};
		ImVec2 m_NextTextSize = {};
};
} /* namespace MiniG::Games */