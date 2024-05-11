#pragma once
#include "common.h"

namespace MiniG
{
class Window
{
	public:
		bool Init(int width, int height);
		void DeInit();

		MGVec2<int> GetSize() const;
		void SetSize(MGVec2<int> new_size);
		GLFWwindow* GetWindow() const;

	private:
		int m_WindowWidth = 0;
		int m_WindowHeight = 0;
		GLFWwindow* m_Window = nullptr;
};
} /* namespace MiniG */