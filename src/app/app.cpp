#include "app.h"
#include "gui/layout.h"
#include "games/tetris.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <exception>
#include <iostream>
#include <chrono>
#include <map>

namespace MiniG
{
std::map<std::string, Games::Game*> g_GamesMap = {
	{"Tetris", new Games::Tetris()},
};

void MainApp::LoadGame(const std::string& game_name)
{
	if(m_CurrentGame)
	{
		m_CurrentGame->OnDetach();
		m_CurrentGame = nullptr;
	}

	if(game_name.empty())
	{
		return;
	}

	try
	{
		m_CurrentGame = g_GamesMap[game_name];
		m_CurrentGame->OnAttach();
	}
	catch(std::exception& e)
	{
		LOG_ERROR("Couldn't find game with name - %s", game_name.c_str());
		throw e; /* TODO: Do we actually need to re-throw exception? Maybe just return to main menu or smth? */
	}
}

bool MainApp::Init()
{
	/* Init OpenGL and ImGui stuff */
	/* FIXME: Read window width and height from some settings file */
	/* TODO: Add asserts */
	if(!m_Window.Init(1600, 900))
	{
		LOG_ERROR("Window init failed!");
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	/* Init imgui platform backends */
	ImGui_ImplGlfw_InitForOpenGL(m_Window.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");

	/* Load default imgui font as the first one */
	ImGui::GetIO().Fonts->AddFontDefault();

	static_cast<Games::Tetris*>(g_GamesMap["Tetris"])->LoadFont();

	return true;
}

void MainApp::Run()
{
	/* Black color */
	glClearColor(0, 0, 0, 1.0);
	auto prev_time = std::chrono::steady_clock::now();

	while(!glfwWindowShouldClose(m_Window.GetWindow()))
	{
		glfwPollEvents();

		/* Start Imgui frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		MGVec2 wnd_size = m_Window.GetSize();
		glViewport(0, 0, wnd_size.x, wnd_size.y);

		/* First draw any GUI than game elements on top */
		/* In most cases this will draw a background while the game itself will draw its GUI */
		Gui::LayoutManager::DrawLayout();
		if(m_CurrentGame)
		{
			const std::chrono::duration<double> delta_time = std::chrono::steady_clock::now() - prev_time;
			m_CurrentGame->OnUpdate(delta_time.count());
		}

		prev_time = std::chrono::steady_clock::now();

		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();

		/* Render end */
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(m_Window.GetWindow());
	}
}
} /* namespace MiniG */