#pragma once
#include "game.h"
#include "app/common.h"

#include <array>

namespace MiniG::Games
{
class Tetris;
class Tetramino
{
	std::vector<MGVec2<int>> OccupiedCells = {};

	friend Tetris;
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
		/* Tetris field is 10 x 20 */
		/* TODO: Idk how really handle this, do we need smth more than a bool per cell? */
		/* Maybe makes sense to have a vector, so when we clear a line we could just remove 1 array from double array? */
		/* Also if nothing more than bool is not needed, than we can use int and set appropriate bits to each cell */
		std::array<std::array<bool, 10>, 20> m_Field = {};

		Tetramino m_FallingTetramino;
};
} /* namespace MiniG::Games */