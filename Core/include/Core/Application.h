#pragma once

// std
#include <string>
#include <memory>
#include <vector>

// libs
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Core
#include "Core/Window.h"
#include "Core/Layer.h"

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

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		void PushLayer()
		{
			m_LayerStack.push_back(std::make_unique<TLayer>());
		}

		glm::vec2 GetFramebufferSize() const;
		std::shared_ptr<Window> GetWindow();

		static Application& Get();
		static double GetTime();

	private:
		ApplicationSpecification m_Specification;
		std::shared_ptr<Window> m_Window;
		bool m_Running = false;

		std::vector<std::unique_ptr<Layer>> m_LayerStack;
	};
}