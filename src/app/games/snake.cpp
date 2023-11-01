#include "snake.h"

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

const double MovementDelay = 0.5;
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

void Snake::processMovement()
{
	if(m_PassedTime < Constant::MovementDelay)
	{
		return;
	}
	m_PassedTime = 0.0;

	/* Move head */
	const MGVec2<int>& direction = g_MovementDirection[m_MovementDirection];
	const MGVec2<int> head_prev_pos = m_HeadPos;
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

	Direction prev_direction = m_MovementDirection;
	
	/* Move the whole body */
	for(size_t i = 0; i < m_SnakeBodyCells.size(); i++)
	{
		MGVec2<int>& cell_pos = m_SnakeBodyCells[i];
		const MGVec2<int>& dir_vec = g_MovementDirection[prev_direction];
		MGVec2 diff = {-1, -1};

		if((i + 1) < m_SnakeBodyCells.size())
		{
			diff = cell_pos - m_SnakeBodyCells[i + 1];
			for(Direction dir = Direction::Up; dir <= Direction::Right; dir = (Direction)((int)dir + 1))
			{
				if(diff == g_MovementDirection[dir])
				{
					prev_direction = dir;
				}
			}
		}

		cell_pos.x += dir_vec.x;
		cell_pos.y += dir_vec.y;
	}

	for(auto cell : m_SnakeBodyCells)
	{
		m_Field[cell.y][cell.x] = CellType::Body;
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