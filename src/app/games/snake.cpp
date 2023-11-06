#include "snake.h"

#include <random>

namespace MiniG::Games
{
namespace Constant
{
const int BaseWindowWidth = 1600;
const int BaseWindowHeight = 900;

const int PaddingYpx = 100;
const int PaddingXpx = 200;

const int RowNumber = 35;
const int ColNumber = 60;

const int FieldWidth = BaseWindowWidth - PaddingXpx * 2;
const int FieldHeight = BaseWindowHeight - PaddingYpx * 2;
const ImVec2 FieldPos = {PaddingXpx, PaddingYpx};
const ImVec2 FieldSize = {FieldWidth, FieldHeight};

const int CellEdgeSize = FieldWidth / ColNumber; /* Should be 35 in both ways */

const MGVec2<int> SnakeHeadStartingPos = {50, 18};
const MGVec2<int> FruitStartingPos = {10, 18};
const MGVec2<int> CellSize = {CellEdgeSize, CellEdgeSize};

const double MovementDelay = 0.25;
} /* namespace Constant */

std::map<CellType, ImVec4> g_CellColor = {
	{CellType::Empty,  ImVec4(  0,   0,   0, 255)},
	{CellType::Head,   ImVec4(102, 255,  51, 255)},
	{CellType::Body,   ImVec4( 51, 204,   0, 255)},
	{CellType::Fruit,  ImVec4(255,  51,   0, 255)},
};

std::map<Direction, MGVec2<int>> g_MovementDirection = {
	{Direction::Up,     { 0, -1}},
	{Direction::Down,   { 0,  1}},
	{Direction::Left,   {-1,  0}},
	{Direction::Right,  { 1,  0}},
};

static void BeginSnakeGUI()
{
	/* Set style for various elements */
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowBorderSize = 1.f;
	style->WindowPadding = ImVec2(0.0f, 0.0f);
	style->Colors[ImGuiCol_Border] = ImVec4(218.0f / 255, 216.0f / 255, 216.0f / 255, 1.0f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); /* Transparrent background */

	ImGui::SetNextWindowPos(Constant::FieldPos);
	ImGui::SetNextWindowSize(Constant::FieldSize);
	ImGui::Begin("Snake GUI", nullptr, ImGuiWindowFlags_NoDecoration);
	
	ImGui::StyleColorsDark(nullptr); /* Bring back 'default' style */
}

static Direction GetOppositeDirection(Direction dir)
{
	switch(dir)
	{
		case Direction::Left:
			return Direction::Right;
		case Direction::Right:
			return Direction::Left;
		case Direction::Up:
			return Direction::Down;
		case Direction::Down:
			return Direction::Up;
	}

	/* We should never reach here */
	throw;
}

void Snake::drawField()
{
	const ImVec2 cell_size = {(float)Constant::CellSize.x, (float)Constant::CellSize.y};
	for(size_t i = 0; i < m_Field.size(); i++)
	{
		for(size_t j = 0; j < m_Field[i].size(); j++)
		{
			const CellType& cell_type = m_Field[i][j];
			if(!m_Textures[cell_type].IsReady())
			{
				continue;
			}

			ImGui::SetCursorPosX((float)(j * Constant::CellEdgeSize));
			ImGui::SetCursorPosY((float)(i * Constant::CellEdgeSize));
			ImGui::Image((void*)(int64_t)m_Textures[cell_type].GetID(), cell_size,
					{0, 0}, {1, 1},
					Vec4Norm(g_CellColor[cell_type], 255)
				);
		}
	}
}

void Snake::generateFruitNewPos()
{
	/* Generate a pool of empty cells */
	std::vector<MGVec2<int>> empty_cells = {};
	for(size_t i = 0; i < m_Field.size(); i++)
	{
		for(size_t j = 0; j < m_Field[i].size(); j++)
		{
			if(m_Field[i][j] == CellType::Empty)
			{
				MGVec2<int> cell_pos = {j, i};
				empty_cells.push_back(cell_pos);
			}
		}
	}

	const int empty_cells_num = (int)empty_cells.size();
	srand(time(nullptr));

	/* Apply the new fruit position */
	m_FruitPos = empty_cells[ rand() % empty_cells_num ];
	m_Field[m_FruitPos.y][m_FruitPos.x] = CellType::Fruit;
}

void Snake::processMovement()
{
	if(m_PassedTime < Constant::MovementDelay)
	{
		return;
	}
	m_PassedTime = 0.0;
	m_HasDirectionChanged = false;

	/* Move head */
	const MGVec2<int>& direction = g_MovementDirection[m_MovementDirection];
	MGVec2<int> head_prev_pos = m_HeadPos;
	m_HeadPos.x += direction.x;
	m_HeadPos.y += direction.y;
	if(m_HeadPos.y >= Constant::RowNumber || m_HeadPos.x >= Constant::ColNumber)
	{
		m_HeadPos = head_prev_pos;
		return;
	}

	m_Field[head_prev_pos.y][head_prev_pos.x] = CellType::Empty;
	m_Field[m_HeadPos.y][m_HeadPos.x] = CellType::Head;

	for(auto cell : m_SnakeBodyCells)
	{
		m_Field[cell.y][cell.x] = CellType::Empty;
	}

	MGVec2<int> last_cell_pos;
	if(m_SnakeBodyCells.size())
	{
		last_cell_pos = m_SnakeBodyCells.back();
	}
	else
	{
		last_cell_pos = head_prev_pos;
	}
	
	/* Move the whole body */
	MGVec2<int> prev_body_pos = {-1, -1};
	for(size_t i = 0; i < m_SnakeBodyCells.size(); i++)
	{
		MGVec2<int>& cell_pos = m_SnakeBodyCells[i];
		MGVec2 diff = {-1, -1};

		/* Compare the first cell with head */
		if(i == 0)
		{
			diff = head_prev_pos - cell_pos;
		}
		else
		{
			diff = prev_body_pos - cell_pos;
		}

		prev_body_pos = cell_pos;

		cell_pos.x += diff.x;
		cell_pos.y += diff.y;
	}

	if(m_ShouldAddBody)
	{
		LOG_DEBUG("Adding the body at position x=%d y=%d", last_cell_pos.x, last_cell_pos.y);
		m_SnakeBodyCells.push_back(last_cell_pos);
		m_ShouldAddBody = false;
	}

	/* Set to true later than the whole process, so it would be updated on the next move */
	if(m_HeadPos == m_FruitPos)
	{
		LOG_DEBUG("We should eat eat the fruit");
		m_ShouldAddBody = true;
		generateFruitNewPos();
	}

	for(auto cell : m_SnakeBodyCells)
	{
		m_Field[cell.y][cell.x] = CellType::Body;
	}
}

void Snake::tryApplyDirection(Direction dir)
{
	if(dir != GetOppositeDirection(m_MovementDirection) && !m_HasDirectionChanged)
	{
		m_MovementDirection = dir;
		m_HasDirectionChanged = true;
	}
}

void Snake::processInput()
{
	/* A or arrow left */
	if(ImGui::IsKeyPressed(ImGuiKey_A) || ImGui::IsKeyPressed(ImGuiKey_LeftArrow))
	{
		tryApplyDirection(Direction::Left);
	}
	/* D or arrow right */
	else if(ImGui::IsKeyPressed(ImGuiKey_D) || ImGui::IsKeyPressed(ImGuiKey_RightArrow))
	{
		tryApplyDirection(Direction::Right);
	}
	/* S or arrow down */
	else if(ImGui::IsKeyPressed(ImGuiKey_S) || ImGui::IsKeyPressed(ImGuiKey_DownArrow))
	{
		tryApplyDirection(Direction::Down);
	}
	else if(ImGui::IsKeyPressed(ImGuiKey_W) || ImGui::IsKeyPressed(ImGuiKey_UpArrow))
	{
		tryApplyDirection(Direction::Up);
	}
}

void Snake::OnAttach()
{
	LOG_DEBUG("Attaching Snake");

	/* Clear field */
	for(auto& row : m_Field)
	{
		for(auto& cell : row)
		{
			cell = CellType::Empty;
		}
	}

	m_PassedTime = 0.0;
	m_HasDirectionChanged = false;
	m_ShouldAddBody = false;

	m_SnakeBodyCells.clear();

	/* Snake will start at specific positions */
	m_HeadPos = Constant::SnakeHeadStartingPos;
	m_FruitPos = Constant::FruitStartingPos;
	m_Field[m_HeadPos.y][m_HeadPos.x] = CellType::Head;
	m_Field[m_FruitPos.y][m_FruitPos.x] = CellType::Fruit;

	m_MovementDirection = Direction::Left;

	/* Create textures */
	m_Textures[CellType::Empty] = Resources::Texture(g_CellColor[CellType::Empty], Constant::CellSize, true);
	if(!m_Textures[CellType::Empty].IsReady())
	{
		LOG_ERROR("Failed to create Empty texture");
	}

	m_Textures[CellType::Head] = Resources::Texture(g_CellColor[CellType::Head], Constant::CellSize, true);
	if(!m_Textures[CellType::Head].IsReady())
	{
		LOG_ERROR("Failed to create Head texture");
	}

	m_Textures[CellType::Body] = Resources::Texture(g_CellColor[CellType::Body], Constant::CellSize, true);
	if(!m_Textures[CellType::Body].IsReady())
	{
		LOG_ERROR("Failed to create Body texture");
	}

	m_Textures[CellType::Fruit] = Resources::Texture(g_CellColor[CellType::Fruit], Constant::CellSize, true);
	if(!m_Textures[CellType::Fruit].IsReady())
	{
		LOG_ERROR("Failed to create Fruit texture");
	}
}

void Snake::OnUpdate(double dt)
{
	m_PassedTime += dt;
	processMovement();
	processInput();

	BeginSnakeGUI();

	drawField();

	/* End window from BeginSnakeGUI() call */
	ImGui::End();
}

void Snake::OnDetach()
{
	LOG_DEBUG("Detaching Snake");
}
} /* namespace MiniG::Games */