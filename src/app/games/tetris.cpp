#include "tetris.h"
#include "app/gui/layout.h"

#include <map>
#include <random>

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
} /* namespace Consts */

std::map<BlockColor, ImVec4> g_BlockColors = {
	{BlockColor::ColorRed,     ImVec4(255,   0,   0, 255)},
	{BlockColor::ColorGreen,   ImVec4(  0, 255,   0, 255)},
	{BlockColor::ColorYellow,  ImVec4(255, 255,   0, 255)},
	{BlockColor::ColorBlue,    ImVec4(  0, 255, 255, 255)},
	{BlockColor::ColorPurple,  ImVec4(255,   0, 255, 255)},
};

std::map<TetraminoShape, std::vector<MGVec2<int>>> g_TetraminoStartingPos = {
	{TetraminoShape::Shape_I, {{5, 0}, {5, 1}, {5, 2}, {5, 3}}},
	{TetraminoShape::Shape_J, {{6, 0}, {6, 1}, {6, 2}, {5, 2}}},
	{TetraminoShape::Shape_L, {{5, 0}, {5, 1}, {5, 2}, {6, 2}}},
	{TetraminoShape::Shape_O, {{5, 0}, {6, 0}, {5, 1}, {6, 1}}},
	{TetraminoShape::Shape_S, {{5, 1}, {6, 1}, {6, 0}, {7, 0}}},
	{TetraminoShape::Shape_T, {{5, 0}, {6, 0}, {7, 0}, {6, 1}}},
	{TetraminoShape::Shape_Z, {{5, 0}, {6, 0}, {6, 1}, {7, 1}}},
};

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
	if(m_BlockTextures[BlockTexture::Wall].IsReady())
	{
		const GLuint wall_texture_id = m_BlockTextures[BlockTexture::Wall].GetID();

		/* Draw left and right walls */
		for(int i = 0; i < (Consts::cPlayFieldHeight); i++)
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

	if(m_BlockTextures[BlockTexture::Block].IsReady())
	{
		const GLuint block_texture_id = m_BlockTextures[BlockTexture::Block].GetID();

		/* Rows */
		for(int i = 0; i < m_Field.size(); i++)
		{
			/* Individual block */
			for(int j = 0; j < m_Field[i].size(); j++)
			{
				Block& block = m_Field[i][j];
				if(!block.IsSet)
				{
					continue;
				}

				ImGui::SetCursorPosX((float)(j * Consts::cBlockEdgeSize));
				ImGui::SetCursorPosY((float)(i * Consts::cBlockEdgeSize));
				ImGui::Image((void*)(int64_t)block_texture_id, block_size,
						{0, 0}, {1, 1},
						Vec4Norm(g_BlockColors[block.Color], 255)
					);
			}
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

	ImGui::End();
}

static std::shared_ptr<Tetramino> GenerateTetramino()
{

}

void Tetris::OnAttach()
{
	LOG_DEBUG("Attaching Tetris");

	m_BlockTextures[BlockTexture::Wall] = Resources::Texture("assets/tetris-wall.png");
	if(!m_BlockTextures[BlockTexture::Wall].IsReady())
	{
		LOG_ERROR("Couldn't load Wall texture!");
	}

	m_BlockTextures[BlockTexture::Block] = Resources::Texture("assets/tetris-block.png");
	if(!m_BlockTextures[BlockTexture::Block].IsReady())
	{
		LOG_ERROR("Couldn't load Block texture!");
	}


}

void Tetris::OnDetach()
{
	LOG_DEBUG("Detaching Tetris");
	for(auto [key, val]: m_BlockTextures)
	{
		val.Delete();
	}
	m_BlockTextures.clear();
}

void Tetris::OnUpdate(double dt)
{
	m_PassedTime += dt;

	/* We will have one big window for all GUI elements for Tetris */
	BeginTetrisGUI();

	drawField();
	drawScoreBoard();

	/* End window from BeginTetrisGUI() call */
	ImGui::End();
}
} /* namespace MiniG::Games */