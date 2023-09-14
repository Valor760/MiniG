#include "app.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <exception>
#include <iostream>

namespace MiniG
{
/* Init basic application settings */
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

	return true;
}

void MainApp::Run()
{
	/* Black color */
	glClearColor(0, 0, 0, 1.0);

	while(!glfwWindowShouldClose(m_Window.GetWindow()))
	{
		glfwPollEvents();

		/* Start Imgui frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		WindowSize wnd_size = m_Window.GetSize();
		glViewport(0, 0, wnd_size.Width, wnd_size.Height);

		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();

		/* Render end */
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(m_Window.GetWindow());
	}
}

MainApp::~MainApp()
{
	m_Window.DeInit();
}
} /* namespace MiniG */