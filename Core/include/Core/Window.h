#pragma once

// std
#include <string>

// libs
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Core {

	struct WindowSpecification
	{
		unsigned int Width = 1280;
		unsigned int Height = 720;
		std::string Title;
		bool VSync = true;
		bool Fullscreen = false;
		bool Borderless = false;
		bool Resizable = true;
		bool Maximized = false;
	};

	class Window
	{
	public:
		Window(const WindowSpecification& spec = WindowSpecification());
		~Window();

		void Create();
		void Destroy();

		void Update();

		glm::vec2 GetFramebufferSize();
		bool ShouldClose() const;

		GLFWwindow* GetHandle() const { return m_WindowHandle; }

	private:
		WindowSpecification m_Specification;
		GLFWwindow* m_WindowHandle = nullptr;
	};
}