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
		Height - 20 playfield + 2 from top and bottom
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

const int cTetrisGuiWidth = 700;
const int cTetrisGuiHeight = 792;
const ImVec2 cTetrisGuiSize = {cTetrisGuiWidth, cTetrisGuiHeight};

const int cTetrisFieldWidth = 432;
const int cTetrisFieldHeight = 792;
const ImVec2 cTetrisFieldSize = {cTetrisFieldWidth, cTetrisFieldHeight};

const int cBlockEdgeSize = 36;
const MGVec2<int> cBlockSize = {cBlockEdgeSize, cBlockEdgeSize};

const int cTetrisScoreBoardWidth = cTetrisGuiWidth - cTetrisFieldWidth;
const int cTetrisScoreBoardHeight = cTetrisGuiHeight;
const ImVec2 cTetrisScoreBoardSize = {cTetrisScoreBoardWidth, cTetrisScoreBoardHeight};

const float cTetrisGuiXPos = (cBaseWindowWidth / 2.f) - (cTetrisGuiWidth / 2.f);
const float cTetrisGuiYPos = (cBaseWindowHeight - cTetrisGuiHeight) / 2.f;
const ImVec2 cTetrisGuiPos = {cTetrisGuiXPos, cTetrisGuiYPos};
const ImVec2 cTetrisFieldPos = cTetrisGuiPos;
const ImVec2 cTetrisScoreBoardPos = {cTetrisGuiXPos + cTetrisFieldWidth, cTetrisGuiYPos};
} /* namespace Consts */

enum class BlockTexture
{
	ColorRed,
	ColorGreen,
	ColorYellow,
	ColorBlue,
	ColorPurple,

	ElementCount
};

/* TODO: This won't be needed later, when the actual texture will be loaded, not the test color */
std::map<BlockTexture, ImVec4> g_BlockColors = {
	{BlockTexture::ColorRed,     ImVec4(253, 180, 180, 255)},
	{BlockTexture::ColorGreen,   ImVec4(128, 255, 170, 255)},
	{BlockTexture::ColorYellow,  ImVec4(255, 255, 153, 255)},
	{BlockTexture::ColorBlue,    ImVec4( 51, 119, 255, 255)},
	{BlockTexture::ColorPurple,  ImVec4(223, 128, 255, 255)},
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

/* TODO: Remove */
const ImVec4 wall_color = ImVec4(242, 242, 242, 255);
Resources::Texture wall_texture;

void Tetris::drawField()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowPadding = ImVec2(0.0f, 0.0f);

	ImGui::SetNextWindowPos(Consts::cTetrisFieldPos);
	ImGui::SetNextWindowSize(Consts::cTetrisFieldSize);
	ImGui::Begin("Tetris Field", nullptr, WINDOW_BACKGROUND_FLAGS);

	/* First, draw the border */
	if(wall_texture.IsReady())
	{
		const GLuint wall_texture_id = wall_texture.GetID();

		/* Draw left and right walls, but not the bottom line*/
		for(int i = 0; i < (Consts::cPlayFieldHeight + 1); i++)
		{
			/* Left block */
			ImGui::SetCursorPos(ImVec2(0, (float)(i * Consts::cBlockEdgeSize)));
			ImGui::Image((void*)(int64_t)wall_texture_id, wall_texture.GetTextureSizeImGui());

			/* Right block */
			ImGui::SetCursorPos(ImVec2(Consts::cTetrisFieldWidth - Consts::cBlockEdgeSize, (float)(i * Consts::cBlockEdgeSize)));
			ImGui::Image((void*)(int64_t)wall_texture_id, wall_texture.GetTextureSizeImGui());
		}

		/* Draw the bottom line */
		for(int i = 0; i < (Consts::cPlayFieldWidth + 2); i++)
		{
			ImGui::SetCursorPos(ImVec2( (float)(i * Consts::cBlockEdgeSize), (float)(Consts::cTetrisFieldHeight - Consts::cBlockEdgeSize) ));
			ImGui::Image((void*)(int64_t)wall_texture_id, wall_texture.GetTextureSizeImGui());
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

void Tetris::OnAttach()
{
	LOG_DEBUG("Attaching Tetris");

	for(int i = 0; i < static_cast<int>(BlockTexture::ElementCount); i++)
	{
		BlockTexture color_idx = static_cast<BlockTexture>(i);
		g_BlockTextures[color_idx] = Resources::Texture(g_BlockColors[color_idx], Consts::cBlockSize, true);
		if(!g_BlockTextures[color_idx].IsReady())
		{
			LOG_ERROR("Failed to load block texture: Color ID=%d", i);
		}
	}

	/* TODO: Test dummy texture, replace with brick texture later */
	wall_texture = Resources::Texture(wall_color, Consts::cBlockSize, true);
	if(!wall_texture.IsReady())
	{
		LOG_ERROR("Failed to load wall texture");
	}
}

void Tetris::OnDetach()
{
	LOG_DEBUG("Detaching Tetris");
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