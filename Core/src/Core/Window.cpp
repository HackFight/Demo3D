#include "Core/Window.h"

#include <glad/glad.h>

#include <iostream>
#include <assert.h>

namespace Core {

	Window::Window(const WindowSpecification& spec)
		: m_Specification(spec)
	{
	}
	Window::~Window()
	{
		Destroy();
	}

	void Window::Create()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		m_WindowHandle = glfwCreateWindow(m_Specification.Width, m_Specification.Height,
			m_Specification.Title.c_str(), nullptr, nullptr);

		if (!m_WindowHandle)
		{
			std::cerr << "Failed to create GLFW window!\n";
			assert(false);
		}
		glfwMakeContextCurrent(m_WindowHandle);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "Failed to initialize GLAD\n";
			assert(false);
		}

		glfwSwapInterval(m_Specification.VSync ? 1 : 0);

		glfwWindowHint(GLFW_SAMPLES, m_Specification.Samples);
		if(m_Specification.Multisampling)
			glEnable(GL_MULTISAMPLE);
	}

	void Window::Destroy()
	{
		if (m_WindowHandle)
			glfwDestroyWindow(m_WindowHandle);

		m_WindowHandle = nullptr;
	}

	void Window::Update()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	glm::vec2 Window::GetFramebufferSize()
	{
		int width, height;
		glfwGetFramebufferSize(m_WindowHandle, &width, &height);
		return { width, height };
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_WindowHandle) != 0;
	}
}