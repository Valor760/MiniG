#include "tetris.h"
#include "app/gui/layout.h"

#include <map>

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
static std::map<BlockTexture, Resources::Texture> g_BlockTextures;

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
	if(g_BlockTextures[BlockTexture::Wall].IsReady())
	{
		const GLuint wall_texture_id = g_BlockTextures[BlockTexture::Wall].GetID();

		/* Draw left and right walls */
		for(int i = 0; i < (Consts::cPlayFieldHeight); i++)
		{
			/* Left block */
			ImGui::SetCursorPos(ImVec2(0, (float)(i * Consts::cBlockEdgeSize)));
			ImGui::Image((void*)(int64_t)wall_texture_id, block_size);

			/* Right block */
			ImGui::SetCursorPos(ImVec2(Consts::cTetrisFieldWidth - Consts::cBlockEdgeSize, (float)(i * Consts::cBlockEdgeSize)));
			ImGui::Image((void*)(int64_t)wall_texture_id, block_size);
		}
	}

	if(g_BlockTextures[BlockTexture::Block].IsReady())
	{

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

void Tetris::OnAttach()
{
	LOG_DEBUG("Attaching Tetris");

	g_BlockTextures[BlockTexture::Wall] = Resources::Texture("assets/tetris-wall.png");
	if(!g_BlockTextures[BlockTexture::Wall].IsReady())
	{
		LOG_ERROR("Couldn't load Wall texture!");
	}

	g_BlockTextures[BlockTexture::Block] = Resources::Texture("assets/tetris-block.png");
	if(!g_BlockTextures[BlockTexture::Block].IsReady())
	{
		LOG_ERROR("Couldn't load Block texture!");
	}
}

void Tetris::OnDetach()
{
	LOG_DEBUG("Detaching Tetris");
	for(auto [key, val]: g_BlockTextures)
	{
		val.Delete();
	}
	g_BlockTextures.clear();
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