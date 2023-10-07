#include "window.h"

namespace MiniG
{
void GLFW_WindowSizeCallback(GLFWwindow* window, int new_width, int new_height)
{
	Window* wnd = static_cast<Window*>(glfwGetWindowUserPointer(window));
	/* TODO: assert wnd != nullptr*/
	MGVec2 prev_size = wnd->GetSize();
	
	LOG_DEBUG("Window size changed: [prev %dx%d] [new %dx%d]", prev_size.x, prev_size.y, new_width, new_height);

	wnd->SetSize({new_width, new_height});
}

bool Window::Init(int width, int height)
{
	LOG_DEBUG("");

	if(width <= 0 || height <= 0)
	{
		LOG_ERROR("Wrong window size provided. width=%d height=%d", width, height);
		return false;
	}
	/*
		TODO: Get screen resolution. If width and height provided are greater than screen res,
		then crop to the screen res.
	*/
	m_WindowWidth = width;
	m_WindowHeight = height;

	if(!glfwInit())
	{
		LOG_ERROR("Can't initialize GLFW");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* TODO: Remove resize completely, allow rezise only using settings */
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// GLFWmonitor* primary = glfwGetPrimaryMonitor();
	// const GLFWvidmode* mode = glfwGetVideoMode(primary);

	/* TODO: Make a file with settings and get window width and height from there */
	/* TODO: Maybe make funny titles like in terraria and minecraft? :) */
	/* TODO: For borderless fullscreen see https://github.com/glfw/glfw/issues/1036#issuecomment-311091384 */
	m_Window = glfwCreateWindow(width, height, "MiniG", nullptr, nullptr);
	// glfwSetWindowMonitor(m_Window, primary, 0, 0, mode->width, mode->height, mode->refreshRate);
	if(m_Window == nullptr)
	{
		LOG_ERROR("Failed to create window");
		return false;
	}
	
	LOG_INFO("Created window=%p size=%dx%d", m_Window, width, height);

	glfwMakeContextCurrent(m_Window);
	/* TODO: Add an option to customize this setting */
	glfwSwapInterval(1); // Enable VSYNC

	glfwSetWindowUserPointer(m_Window, this);
	glfwSetWindowSizeCallback(m_Window, GLFW_WindowSizeCallback);

	return true;
}

void Window::DeInit()
{
	LOG_DEBUG("");

	if(m_Window)
	{
		LOG_DEBUG("Destroying window=%p", m_Window);
		glfwDestroyWindow(m_Window);
	}
}

MGVec2<int> Window::GetSize() const
{
	return {m_WindowWidth, m_WindowHeight};
}

void Window::SetSize(MGVec2<int> new_size)
{
	LOG_DEBUG("New size=%dx%d", new_size.x, new_size.y);

	m_WindowWidth = new_size.x;
	m_WindowHeight = new_size.y;
}

GLFWwindow* Window::GetWindow() const
{
	return m_Window;
}
} /* namespace MiniG */