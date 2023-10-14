#include "tetris.h"
#include "app/gui/layout.h"

#include <random>
#include <algorithm>

namespace MiniG::Games
{
/*
	CALCULATIONS:
	Assuming the whole window is 1600 by 900.
	The Tetris GUI window will be 700 width and 792 height.
	Tetris field should be 432 width and 792 height(the same as GUI).
	Number of blocks in the field:
		Width - 10 playfield + 2 on the sides
		Height - 20 playfield
	Each block size is 36;

	Window to the right(scoring, next piece etc):
	Width is Tetris GUI minus Tetris field(700 - 432 = 268)
	Height is the same as GUI
*/
namespace Consts
{
/* TODO: How to change this according to current resolution? (probably make a multipliers) */
const int cBaseWindowWidth = 1600;
const int cBaseWindowHeight = 900;
const int cPlayFieldWidth = 10;
const int cPlayFieldHeight = 20;

const int cBlockEdgeSize = 36;
const MGVec2<int> cBlockSize = {cBlockEdgeSize, cBlockEdgeSize};

const int cTetrisGuiWidth = 700;
const int cTetrisGuiHeight = cBlockEdgeSize * (cPlayFieldHeight);
const ImVec2 cTetrisGuiSize = {cTetrisGuiWidth, cTetrisGuiHeight};

const int cTetrisFieldWidth = cBlockEdgeSize * (cPlayFieldWidth + 2);
const int cTetrisFieldHeight = cTetrisGuiHeight;
const ImVec2 cTetrisFieldSize = {cTetrisFieldWidth, cTetrisFieldHeight};

const int cTetrisScoreBoardWidth = cTetrisGuiWidth - cTetrisFieldWidth;
const int cTetrisScoreBoardHeight = cTetrisGuiHeight;
const ImVec2 cTetrisScoreBoardSize = {cTetrisScoreBoardWidth, cTetrisScoreBoardHeight};

const float cTetrisGuiXPos = (cBaseWindowWidth / 2.f) - (cTetrisGuiWidth / 2.f);
const float cTetrisGuiYPos = (cBaseWindowHeight - cTetrisGuiHeight) / 2.f;
const ImVec2 cTetrisGuiPos = {cTetrisGuiXPos, cTetrisGuiYPos};
const ImVec2 cTetrisFieldPos = cTetrisGuiPos;
const ImVec2 cTetrisScoreBoardPos = {cTetrisGuiXPos + cTetrisFieldWidth, cTetrisGuiYPos};

const double cFallTimeDelaySec = 1.0;
} /* namespace Consts */

std::map<BlockColor, ImVec4> g_BlockColors = {
	{BlockColor::ColorRed,     ImVec4(255,   0,   0, 255)},
	{BlockColor::ColorGreen,   ImVec4(  0, 255,   0, 255)},
	{BlockColor::ColorYellow,  ImVec4(255, 255,   0, 255)},
	{BlockColor::ColorBlue,    ImVec4(  0, 255, 255, 255)},
	{BlockColor::ColorPurple,  ImVec4(255,   0, 255, 255)},
};

std::map<TetraminoShape, std::vector<MGVec2<int>>> g_TetraminoStartingPos = {
	{TetraminoShape::Shape_I, {{4, 0}, {4, 1}, {4, 2}, {4, 3}}},
	{TetraminoShape::Shape_J, {{5, 0}, {5, 1}, {5, 2}, {4, 2}}},
	{TetraminoShape::Shape_L, {{4, 0}, {4, 1}, {4, 2}, {5, 2}}},
	{TetraminoShape::Shape_O, {{4, 0}, {5, 0}, {4, 1}, {5, 1}}},
	{TetraminoShape::Shape_S, {{4, 1}, {5, 1}, {5, 0}, {6, 0}}},
	{TetraminoShape::Shape_T, {{4, 0}, {5, 0}, {6, 0}, {5, 1}}},
	{TetraminoShape::Shape_Z, {{4, 0}, {5, 0}, {5, 1}, {6, 1}}},
};

std::map<TetraminoShape, MGVec2<int>> g_TetraminoMaxDimensions = {
	{TetraminoShape::Shape_I, {1, 4}},
	{TetraminoShape::Shape_J, {2, 3}},
	{TetraminoShape::Shape_L, {2, 3}},
	{TetraminoShape::Shape_O, {2, 2}},
	{TetraminoShape::Shape_S, {3, 2}},
	{TetraminoShape::Shape_T, {3, 2}},
	{TetraminoShape::Shape_Z, {3, 2}},
};

std::uniform_int_distribution<int> g_ShapeRandGen((int)TetraminoShape::Shape_I, (int)TetraminoShape::Shape_Z);
std::uniform_int_distribution<int> g_ColorRandGen((int)BlockColor::ColorRed, (int)BlockColor::ColorPurple);

static void BeginTetrisGUI()
{
	/* Set style for various elements */
	/* TODO: Maybe extract this to separate function */
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowBorderSize = 0.1f;
	style->Colors[ImGuiCol_Border] = ImVec4(218.0f / 255, 216.0f / 255, 216.0f / 255, 1.0f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); /* Transparrent background */

	ImGui::SetNextWindowPos(Consts::cTetrisGuiPos);
	ImGui::SetNextWindowSize(Consts::cTetrisGuiSize);
	ImGui::Begin("Tetris GUI", nullptr, ImGuiWindowFlags_NoDecoration);
	
	ImGui::StyleColorsDark(nullptr); /* Bring back 'default' style */
}

static inline ImVec4 Vec4Norm(const ImVec4& vec4, int norm)
{
	return ImVec4(
			vec4.x / norm,
			vec4.y / norm,
			vec4.z / norm,
			vec4.w / norm
		);
}

void Tetris::drawField()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowPadding = ImVec2(0.0f, 0.0f);

	ImGui::SetNextWindowPos(Consts::cTetrisFieldPos);
	ImGui::SetNextWindowSize(Consts::cTetrisFieldSize);
	ImGui::Begin("Tetris Field", nullptr, WINDOW_BACKGROUND_FLAGS);

	const ImVec2 block_size = ImVec2(Consts::cBlockEdgeSize, Consts::cBlockEdgeSize);

	/* First, draw the border */
	if(m_Textures[Texture::Wall].IsReady())
	{
		const GLuint wall_texture_id = m_Textures[Texture::Wall].GetID();

		/* Draw left and right walls */
		for(int i = 0; i < Consts::cPlayFieldHeight; i++)
		{
			/* Left block */
			ImGui::SetCursorPosX(0);
			ImGui::SetCursorPosY((float)(i * Consts::cBlockEdgeSize));
			ImGui::Image((void*)(int64_t)wall_texture_id, block_size);

			/* Right block */
			ImGui::SetCursorPosX(Consts::cTetrisFieldWidth - Consts::cBlockEdgeSize);
			ImGui::SetCursorPosY((float)(i * Consts::cBlockEdgeSize));
			ImGui::Image((void*)(int64_t)wall_texture_id, block_size);
		}
	}

	/* Draw field background */
	if(m_Textures[Texture::Block].IsReady())
	{
		for(int i = 0; i < Consts::cPlayFieldHeight; i++)
		{
			for(int j = 0; j < Consts::cPlayFieldWidth; j++)
			{
				/* X offsets by 1 because of walls */
				ImGui::SetCursorPosX((float)((j + 1) * Consts::cBlockEdgeSize));
				ImGui::SetCursorPosY((float)(i * Consts::cBlockEdgeSize));
				ImGui::Image((void*)(int64_t)m_Textures[Texture::Block].GetID(), block_size,
						{0, 0}, {1, 1},
						Vec4Norm(ImVec4(40.f, 40.f, 40.f, 255.f), 255)
					);
			}
		}
	}

	/* Draw tetramino projection */
	/* Draw this BEFORE the tetramino */
	if(m_Textures[Texture::BlockProjection].IsReady())
	{
		int y_diff = findLowestAllowedBlock();

		/* Draw the projection */
		for(const auto& block_pos : m_FallingTetramino->OccupiedCells)
		{
			/* X offsets by 1 because of walls */
			ImGui::SetCursorPosX((float)((block_pos.x + 1) * Consts::cBlockEdgeSize));
			ImGui::SetCursorPosY((float)((block_pos.y + y_diff) * Consts::cBlockEdgeSize));
			ImGui::Image((void*)(int64_t)m_Textures[Texture::BlockProjection].GetID(), block_size,
					{0, 0}, {1, 1},
					Vec4Norm(g_BlockColors[m_FallingTetramino->Color], 255)
				);
		}
	}

	if(m_Textures[Texture::Block].IsReady())
	{
		const GLuint block_texture_id = m_Textures[Texture::Block].GetID();

		/* Draw the field */
		/* Rows */
		for(size_t i = 0; i < m_Field.size(); i++)
		{
			/* Individual block */
			for(size_t j = 0; j < m_Field[i].size(); j++)
			{
				Block& block = m_Field[i][j];
				if(!block.IsSet)
				{
					continue;
				}

				/* X offsets by 1 because of walls */
				ImGui::SetCursorPosX((float)((j + 1) * Consts::cBlockEdgeSize));
				ImGui::SetCursorPosY((float)(i * Consts::cBlockEdgeSize));
				ImGui::Image((void*)(int64_t)block_texture_id, block_size,
						{0, 0}, {1, 1},
						Vec4Norm(g_BlockColors[block.Color], 255)
					);
			}
		}

		/* Draw the falling tetramino */
		for(const auto& block_coords : m_FallingTetramino->OccupiedCells)
		{
			/* X offsets by 1 because of walls */
			ImGui::SetCursorPosX((float)((block_coords.x + 1) * Consts::cBlockEdgeSize));
			ImGui::SetCursorPosY((float)(block_coords.y * Consts::cBlockEdgeSize));
			ImGui::Image((void*)(int64_t)block_texture_id, block_size,
					{0, 0}, {1, 1},
					Vec4Norm(g_BlockColors[m_FallingTetramino->Color], 255)
				);
		}
	}

	ImGui::End();

	/* Set style back as it was */
	ImGui::StyleColorsDark(nullptr);
}

void Tetris::drawScoreBoard()
{
	ImGui::SetNextWindowPos(Consts::cTetrisScoreBoardPos);
	ImGui::SetNextWindowSize(Consts::cTetrisScoreBoardSize);
	ImGui::Begin("Tetris ScoreBoard", nullptr, WINDOW_BACKGROUND_FLAGS);

	/* Draw scoreboard background */
	const int width_blocks = 6;
	const GLuint block_texture_id = m_Textures[Texture::Block].GetID();
	const float block_edge_size = Consts::cTetrisScoreBoardWidth / (float)width_blocks;
	const ImVec2 block_size = ImVec2(block_edge_size, block_edge_size);
	for(int i = 0; i < Consts::cPlayFieldHeight; i++)
	{
		for(int j = 0; j < width_blocks; j++)
		{
			ImVec4 color = {50.f, 50.f, 50.f, 255.f};
			if(i >= 10 && i <= 13 && j >= 1 && j <= 4)
			{
				/* Window background is transparrent. Draw an invisible black texture */
				color = {0.f, 0.f, 0.f, 255.f};
			}

			ImGui::SetCursorPosX((float)(j * block_edge_size));
			ImGui::SetCursorPosY((float)(i * block_edge_size));
			ImGui::Image((void*)(int64_t)block_texture_id, block_size,
					{0, 0}, {1, 1},
					Vec4Norm(color, 255)
				);
		}
	}

	ImGui::PushFont(m_ScoreBoardFont);
	{
		/* Move text a little */
		float score_cursor_x = (Consts::cTetrisScoreBoardWidth - m_ScoreTextSize.x) / 2.0f;
		ImGui::SetCursorPos({score_cursor_x + 5.0f, 110.0f});
		ImGui::Text("SCORE");
	}

	{
		/* TODO: Do we need to dynamically update width and offset of the score? */
		ImVec2 next_pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos({next_pos.x + 8.f, next_pos.y});
		ImGui::Text("%06d", m_Score);
	}

	{
		ImVec2 next_pos = ImGui::GetCursorPos();
		float next_cursor_x = (Consts::cTetrisScoreBoardWidth - m_NextTextSize.x) / 2.0f;
		ImGui::SetCursorPos({next_cursor_x + 5.0f, next_pos.y + 150.0f});
		ImGui::Text("NEXT");
	}

	ImGui::PopFont();

	/* Draw next tetramino */
	{
		ImVec2 prev_pos = ImGui::GetCursorPos();
		int next_block_y = (int)(prev_pos.y / block_edge_size + 1);
		const MGVec2<int>& tetramino_dim = g_TetraminoMaxDimensions[m_NextTetramino->Shape];
		float x_offset = (4.f - tetramino_dim.x) / 2.f;
		float y_offset = (4.f - tetramino_dim.y) / 2.f;
		for(const auto& block_coord : m_NextTetramino->OccupiedCells)
		{
			ImGui::SetCursorPosX(block_edge_size * (block_coord.x - 3) + (x_offset * block_edge_size));
			ImGui::SetCursorPosY(block_edge_size * (block_coord.y + next_block_y) + (y_offset * block_edge_size));
			ImGui::Image((void*)(int64_t)block_texture_id, block_size,
				{0, 0}, {1, 1},
				Vec4Norm(g_BlockColors[m_NextTetramino->Color], 255)
			);
		}
	}

	ImGui::End();
}

static std::shared_ptr<Tetramino> GenerateTetramino()
{
	/* Reinit random generator on every tetramino creation */
	std::mt19937 gen((std::random_device())());

	TetraminoShape shape = (TetraminoShape)g_ShapeRandGen(gen);
	BlockColor color = (BlockColor)g_ColorRandGen(gen);

	std::shared_ptr<Tetramino> new_tetramino = std::make_shared<Tetramino>();
	new_tetramino->Shape = shape;
	new_tetramino->OccupiedCells = g_TetraminoStartingPos[shape];
	new_tetramino->Color = color;

	return new_tetramino;
}

void Tetris::applyTetraminoToField()
{
	/* Apply current blocks to field */
	for(auto& block_coords : m_FallingTetramino->OccupiedCells)
	{
		Block& field_block = m_Field[block_coords.y][block_coords.x];
		assert(!field_block.IsSet);
		field_block.Color = m_FallingTetramino->Color;
		field_block.IsSet = true;
	}

	m_FallingTetramino = m_NextTetramino;
	m_NextTetramino = GenerateTetramino();
}

void Tetris::TimeMoveFallingTetramino()
{
	/* Check if the tetramino can be moved further */
	bool canBeMoved = true;
	std::vector<MGVec2<int>> downmost_coords = m_FallingTetramino->OccupiedCells;

	/* Check with field, that movement is allowed */
	for(const auto& block_coords : downmost_coords)
	{
		if(static_cast<size_t>(block_coords.y + 1) >= m_Field.size() || m_Field[block_coords.y + 1][block_coords.x].IsSet)
		{
			canBeMoved = false;
			break;
		}
	}

	if(canBeMoved)
	{
		LOG_DEBUG("Moving tetramino down");
		for(auto& block_coords : m_FallingTetramino->OccupiedCells)
		{
			block_coords.y += 1;
		}
	}
	else
	{
		LOG_DEBUG("Current tetramino cannot be moved further down");
		applyTetraminoToField();
	}
}

void Tetris::MoveFallingTetraminoToSide(int where)
{
	/* Check if the tetramino can be moved further */
	bool canBeMoved = true;
	std::vector<MGVec2<int>> sidemost_coords = m_FallingTetramino->OccupiedCells;

	/* Check with field, that movement is allowed */
	for(const auto& block_coords : sidemost_coords)
	{
		if((block_coords.x + where) < 0 || static_cast<size_t>(block_coords.x + where) >= m_Field[0].size())
		{
			canBeMoved = false;
			break;
		}

		if(m_Field[block_coords.y][block_coords.x + where].IsSet)
		{
			canBeMoved = false;
			break;
		}
	}

	if(canBeMoved)
	{
		LOG_DEBUG("Moving tetramino to the %s", where == MoveAction::ToLeft ? "left" : "right");
		for(auto& block_coords : m_FallingTetramino->OccupiedCells)
		{
			block_coords.x += where;
		}
	}
}

int Tetris::findLowestAllowedBlock()
{
	std::vector<MGVec2<int>> downmost_coords = m_FallingTetramino->OccupiedCells;

	/* Find the nearest occupied block on the field and calculate minimum difference */
	int nearest_field_y_diff = 9999; /* Dummy value */
	for(const auto& block_coord : downmost_coords)
	{
		for(size_t i = block_coord.y + 1; i < m_Field.size(); i++)
		{
			if(!m_Field[i][block_coord.x].IsSet)
			{
				continue;
			}

			int coord_diff = (int)i - block_coord.y - 1;
			if(coord_diff < nearest_field_y_diff)
			{
				nearest_field_y_diff = coord_diff;
			}
		}
	}

	/* No blocks are set underneith. Find lowest point of the tetramino */
	if(static_cast<size_t>(nearest_field_y_diff) >= m_Field.size())
	{
		/* "Lowest" is a little wrong term. In our case Bottom line is y = 20 */
		int lowest_y_point = -1;
		for(const auto& block_coord : downmost_coords)
		{
			if(block_coord.y > lowest_y_point)
			{
				lowest_y_point = block_coord.y;
			}
		}

		assert(lowest_y_point != -1);

		nearest_field_y_diff = (int)m_Field.size() - 1 - lowest_y_point;
	}

	return nearest_field_y_diff;
}

void Tetris::DropFallingTetramino()
{
	int nearest_field_y_diff = findLowestAllowedBlock();

	/* Move falling tetramino down by calculated Y diff */
	for(auto& block_coord : m_FallingTetramino->OccupiedCells)
	{
		block_coord.y += nearest_field_y_diff;
	}

	applyTetraminoToField();
}

void Tetris::rorateFallingTetramino()
{
	/* Yeah, everything is hardcoded, but whatever, I don't know a general way how to rotate them */
	std::vector<MGVec2<int>> blocks_vec = m_FallingTetramino->OccupiedCells;
	std::vector<MGVec2<int>> rotation_angle(4);

	switch(m_FallingTetramino->Shape)
	{
		case TetraminoShape::Shape_I:
		{
			if(m_FallingTetramino->RotationState == 0)
			{
				/*
				[]              
				[]              
				[]  ->  [][][][]
				[]              
				*/
				rotation_angle[0] = {2, 2};
				rotation_angle[1] = {1, 1};
				rotation_angle[2] = {0, 0};
				rotation_angle[3] = {-1, -1};
			}
			else
			{
				/*
				              []
				              []
				[][][][]  ->  []
				              []
				*/
				m_FallingTetramino->RotationState = 0;
				rotation_angle[0] = {-2, -2};
				rotation_angle[1] = {-1, -1};
				rotation_angle[2] = {0, 0};
				rotation_angle[3] = {1, 1};
				break;
			}

			m_FallingTetramino->RotationState++;
			break;
		}
		case TetraminoShape::Shape_J:
		{
			if(m_FallingTetramino->RotationState == 0)
			{
				/*
				  []            
				  []  ->  []    
				[][]      [][][]
				*/
				rotation_angle[0] = {1, 1};
				rotation_angle[1] = {0, 0};
				rotation_angle[2] = {-1, -1};
				rotation_angle[3] = {0, -2};
			}
			else if(m_FallingTetramino->RotationState == 1)
			{
				/*
				            [][]
				[]      ->  []  
				[][][]      []  
				*/
				rotation_angle[0] = {-1, 1};
				rotation_angle[1] = {0, 0};
				rotation_angle[2] = {1, -1};
				rotation_angle[3] = {2, 0};
			}
			else if(m_FallingTetramino->RotationState == 2)
			{
				/*
				[][]            
				[]    ->  [][][]
				[]            []
				*/
				rotation_angle[0] = {-1, -1};
				rotation_angle[1] = {0, 0};
				rotation_angle[2] = {1, 1};
				rotation_angle[3] = {0, 2};
			}
			else
			{
				/*
				              []
				[][][]  ->    []
				    []      [][]
				*/
				m_FallingTetramino->RotationState = 0;
				rotation_angle[0] = {1, -1};
				rotation_angle[1] = {0, 0};
				rotation_angle[2] = {-1, 1};
				rotation_angle[3] = {-2, 0};
				break;
			}

			m_FallingTetramino->RotationState++;
			break;
		}
		case TetraminoShape::Shape_L:
		{
			if(m_FallingTetramino->RotationState == 0)
			{
				/*
				[]              
				[]    ->  [][][]
				[][]      []    
				*/
				rotation_angle[0] = {1, 1};
				rotation_angle[1] = {0, 0};
				rotation_angle[2] = {-1, -1};
				rotation_angle[3] = {-2, 0};
			}
			else if(m_FallingTetramino->RotationState == 1)
			{
				/*
				            [][]
				[][][]  ->    []
				[]            []
				*/
				rotation_angle[0] = {-1, 1};
				rotation_angle[1] = {0, 0};
				rotation_angle[2] = {1, -1};
				rotation_angle[3] = {0, -2};
			}
			else if(m_FallingTetramino->RotationState == 2)
			{
				/*
				[][]            []
				  []    ->  [][][]
				  []              
				*/
				rotation_angle[0] = {-1, -1};
				rotation_angle[1] = {0, 0};
				rotation_angle[2] = {1, 1};
				rotation_angle[3] = {2, 0};
			}
			else
			{
				/*
				    []        []  
				[][][]  ->    []  
				              [][]
				*/
				m_FallingTetramino->RotationState = 0;
				rotation_angle[0] = {1, -1};
				rotation_angle[1] = {0, 0};
				rotation_angle[2] = {-1, 1};
				rotation_angle[3] = {0, 2};
				break;
			}

			m_FallingTetramino->RotationState++;
			break;
		}
		case TetraminoShape::Shape_O:
		{
			/* No need to rotate */
			break;
		}
		case TetraminoShape::Shape_S:
		{
			if(m_FallingTetramino->RotationState == 0)
			{
				/*
				  [][]      []  
				[][]    ->  [][]
				              []
				*/
				rotation_angle[0] = {1, -1};
				rotation_angle[1] = {0, 0};
				rotation_angle[2] = {1, 1};
				rotation_angle[3] = {0, 2};
			}
			else
			{
				/*
				[]              
				[][]  ->    [][]
				  []      [][]  
				*/
				m_FallingTetramino->RotationState = 0;
				rotation_angle[0] = {-1, 1};
				rotation_angle[1] = {0, 0};
				rotation_angle[2] = {-1, -1};
				rotation_angle[3] = {0, -2};
				break;
			}

			m_FallingTetramino->RotationState++;
			break;
		}
		case TetraminoShape::Shape_T:
		{
			if(m_FallingTetramino->RotationState == 0)
			{
				/*
				              []
				[][][]  ->  [][]
				  []          []
				*/
				rotation_angle[0] = {1, -1};
				rotation_angle[1] = {0, 0};
				rotation_angle[2] = {-1, 1};
				rotation_angle[3] = {-1, -1};
			}
			else if(m_FallingTetramino->RotationState == 1)
			{
				/*
				  []        []  
				[][]  ->  [][][]
				  []            
				*/
				rotation_angle[0] = {1, 1};
				rotation_angle[1] = {0, 0};
				rotation_angle[2] = {-1, -1};
				rotation_angle[3] = {1, -1};
			}
			else if(m_FallingTetramino->RotationState == 2)
			{
				/*
				  []        []  
				[][][]  ->  [][]
				            []  
				*/
				rotation_angle[0] = {-1, 1};
				rotation_angle[1] = {0, 0};
				rotation_angle[2] = {1, -1};
				rotation_angle[3] = {1, 1};
			}
			else
			{
				/*
				[]  
				[][]  ->  [][][]
				[]          []
				*/
				m_FallingTetramino->RotationState = 0;
				rotation_angle[0] = {-1, -1};
				rotation_angle[1] = {0, 0};
				rotation_angle[2] = {1, 1};
				rotation_angle[3] = {-1, 1};
				break;
			}

			m_FallingTetramino->RotationState++;
			break;
		}
		case TetraminoShape::Shape_Z:
		{
			if(m_FallingTetramino->RotationState == 0)
			{
				/*
				[][]          []
				  [][]  ->  [][]
				            []  
				*/
				rotation_angle[0] = {2, 0};
				rotation_angle[1] = {1, 1};
				rotation_angle[2] = {0, 0};
				rotation_angle[3] = {-1, 1};
			}
			else
			{
				/*
				  []      [][]  
				[][]  ->    [][]
				[]              
				*/
				m_FallingTetramino->RotationState = 0;
				rotation_angle[0] = {-2, 0};
				rotation_angle[1] = {-1, -1};
				rotation_angle[2] = {0, 0};
				rotation_angle[3] = {1, -1};
				break;
			}

			m_FallingTetramino->RotationState++;
			break;
			break;
		}
		default:
			throw std::runtime_error("Unknown shape received: " + std::to_string((int)m_FallingTetramino->Shape));
	}

	for(int i = 0; i < 4; i++)
	{
		blocks_vec[i] = {blocks_vec[i].x + rotation_angle[i].x, blocks_vec[i].y + rotation_angle[i].y};
	}

	/* Check for collision */
	MGVec2<int> min_coords = {0, 0};
	MGVec2<int> max_coords = {0, 0};
	for(const auto& block_coords : blocks_vec)
	{
		if(block_coords.x < min_coords.x)
		{
			min_coords.x = block_coords.x;
		}

		if(block_coords.y < min_coords.y)
		{
			min_coords.y = block_coords.y;
		}

		if(block_coords.x > max_coords.x)
		{
			max_coords.x = block_coords.x;
		}

		if(block_coords.x > max_coords.x)
		{
			max_coords.x = block_coords.x;
		}
	}

	MGVec2<int> offset_coords = {0, 0};
	if(min_coords.x < 0)
	{
		offset_coords.x = -min_coords.x;
	}

	if(min_coords.y < 0)
	{
		offset_coords.y = -min_coords.y;
	}

	if(max_coords.x >= 9)
	{
		assert(offset_coords.x == 0);
		offset_coords.x = 9 - max_coords.x;
	}

	if(max_coords.y >= 19)
	{
		assert(offset_coords.y == 0);
		offset_coords.y = 19 - max_coords.y;
	}

	/* Apply offset */
	for(auto& block_coords : blocks_vec)
	{
		block_coords.x += offset_coords.x;
		block_coords.y += offset_coords.y;
	}

	/* Check with the field */
	for(const auto& block_coords : blocks_vec)
	{
		if(m_Field[block_coords.y][block_coords.x].IsSet)
		{
			/* Do not rotate tetramino if it is blocked */
			return;
		}
	}

	m_FallingTetramino->OccupiedCells = blocks_vec;
}

void Tetris::ProcessInput()
{
	/* TODO: Let imgui handle the repeat rate if the button is hold */
	// ImGuiIO& io = ImGui::GetIO();
	// io.KeyRepeatRate = 0.5f;

	bool moveLeft = false;
	bool moveRight = false;

	/* A or arrow left */
	if(ImGui::IsKeyPressed(ImGuiKey_A) || ImGui::IsKeyPressed(ImGuiKey_LeftArrow))
	{
		moveLeft = true;
	}

	/* D or arrow right */
	if(ImGui::IsKeyPressed(ImGuiKey_D) || ImGui::IsKeyPressed(ImGuiKey_RightArrow))
	{
		moveRight = true;
	}

	/* S or arrow down */
	if(ImGui::IsKeyPressed(ImGuiKey_S) || ImGui::IsKeyPressed(ImGuiKey_DownArrow))
	{
		/* Increase passed time by a little bit to quicken the fall */
		m_PassedTime += Consts::cFallTimeDelaySec * 0.75;
	}

	/* Drop tetramino on Space */
	if(ImGui::IsKeyPressed(ImGuiKey_Space, false))
	{
		DropFallingTetramino();

		/* Tetramino has fallen apply it to the field */
		m_PassedTime = 0.0;
		return;
	}

	/* Rotate tetramino */
	if(ImGui::IsKeyPressed(ImGuiKey_R, false))
	{
		rorateFallingTetramino();
	}

	if(moveLeft && moveRight)
	{
		/* Both buttons are pressed, do nothing */
		return;
	}
	else if(moveLeft && !moveRight)
	{
		MoveFallingTetraminoToSide(MoveAction::ToLeft);
	}
	else if(!moveLeft && moveRight)
	{
		MoveFallingTetraminoToSide(MoveAction::ToRight);
	}
}

void Tetris::checkAndRemoveLines()
{
	int lines_removed = 0;

	for(size_t i = 0; i < m_Field.size(); i++)
	{
		/* Check for line completeness */
		bool is_line_complete = true;
		for(const auto& block : m_Field[i])
		{
			if(!block.IsSet)
			{
				is_line_complete = false;
				break;
			}
		}

		if(!is_line_complete)
		{
			continue;
		}

		/* TODO: Add animation in the future if possible */
		for(size_t j = i; j > 0; j--)
		{
			m_Field[j] = m_Field[j - 1];
		}
		m_Field[0] = std::array<Block, 10>{};

		lines_removed++;
		i--;
	}

	if(lines_removed > 0)
	{
		m_Score += lines_removed * lines_removed * 100;
	}
}

void Tetris::LoadFont()
{
	const char* font = "assets/Score-Font.ttf";
	LOG_DEBUG("Loading font (%s)", font);
	ImGuiIO& io = ImGui::GetIO();
	m_ScoreBoardFont = io.Fonts->AddFontFromFileTTF(font, 50.0f);
	if(!m_ScoreBoardFont)
	{
		LOG_ERROR("Failed to load font (%s)", font);
		throw;
	}
	
	LOG_DEBUG("Font successfully loaded");
}

void Tetris::OnAttach()
{
	LOG_DEBUG("Attaching Tetris");

	m_Textures[Texture::Wall] = Resources::Texture("assets/tetris-wall.png");
	if(!m_Textures[Texture::Wall].IsReady())
	{
		LOG_ERROR("Couldn't load Wall texture!");
	}

	m_Textures[Texture::Block] = Resources::Texture("assets/tetris-block.png");
	if(!m_Textures[Texture::Block].IsReady())
	{
		LOG_ERROR("Couldn't load Block texture!");
	}

	m_Textures[Texture::BlockProjection] = Resources::Texture("assets/tetris-block-proj.png");
	if(!m_Textures[Texture::BlockProjection].IsReady())
	{
		LOG_ERROR("Couldn't load BlockProjection texture!");
	}

	m_FallingTetramino = GenerateTetramino();
	m_NextTetramino = GenerateTetramino();

	const char score_text[] = "SCORE";
	const char next_text[] = "NEXT";
	m_ScoreTextSize = m_ScoreBoardFont->CalcTextSizeA(50.0f, FLT_MAX, 0.0f, &score_text[0], &score_text[5]);
	m_NextTextSize  = m_ScoreBoardFont->CalcTextSizeA(50.0f, FLT_MAX, 0.0f, &next_text[0], &next_text[4]);
}

void Tetris::OnDetach()
{
	LOG_DEBUG("Detaching Tetris");

	/* Release all textures */
	for(auto [key, val]: m_Textures)
	{
		val.Delete();
	}
	m_Textures.clear();

	m_FallingTetramino.reset();
	m_NextTetramino.reset();

	for(auto& field_row : m_Field)
	{
		for(auto& block : field_row)
		{
			block.IsSet = false;
			block.Color = BlockColor::ElementCount;
		}
	}

	m_PassedTime = 0.0;
}

void Tetris::OnUpdate(double dt)
{
	m_PassedTime += dt;

	ProcessInput();

	if(m_PassedTime >= Consts::cFallTimeDelaySec)
	{
		m_PassedTime = 0.0;
		TimeMoveFallingTetramino();
	}

	checkAndRemoveLines();

	/* We will have one big window for all GUI elements for Tetris */
	BeginTetrisGUI();

	drawField();
	drawScoreBoard();

	/* End window from BeginTetrisGUI() call */
	ImGui::End();
}
} /* namespace MiniG::Games */