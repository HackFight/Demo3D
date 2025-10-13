#pragma once

#include "Core/Window.h"

// std
#include <string>
#include <memory>

// libs
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Core {

	struct ApplicationSpecification
	{
		std::string Name = "Application";
		WindowSpecification WindowSpec;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& spec = ApplicationSpecification());
		~Application();

		void Run();
		void Stop();

		glm::vec2 GetFramebufferSize() const;

		static Application& Get();
		static float GetTime();

	private:
		ApplicationSpecification m_Specification;
		std::shared_ptr<Window> m_Window;
		bool m_Running = false;
	};
}