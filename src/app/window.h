#pragma once
#include "common.h"

#include <GLFW/glfw3.h>

namespace MiniG
{
struct WindowSize
{
	int Width;
	int Height;
};

class Window
{
	public:
		bool Init(int width, int height);
		void DeInit();

		WindowSize GetSize() const;
		void SetSize(WindowSize new_size);
		GLFWwindow* GetWindow() const;

	private:
		int m_WindowWidth = 0;
		int m_WindowHeight = 0;
		GLFWwindow* m_Window = nullptr;
};
} /* namespace MiniG */