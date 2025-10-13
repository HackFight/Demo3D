#include "Application.h"

#include "Renderer/GLUtils.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <assert.h>
#include <iostream>

namespace Core {

	static Application* s_Application = nullptr;

	static void GLFWErrorCallback(int error, const char* description)
	{
		std::cerr << "[GLFW Error]: " << description << std::endl;
	}

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		s_Application = this;

		glfwSetErrorCallback(GLFWErrorCallback);
		glfwInit();

		// Set window title to app name if empty
		if (m_Specification.WindowSpec.Title.empty())
			m_Specification.WindowSpec.Title = m_Specification.Name;

		m_Window = std::make_shared<Window>(m_Specification.WindowSpec);
		m_Window->Create();

		Renderer::Utils::InitOpenGLDebugMessageCallback();
	}

	Application::~Application()
	{
		m_Window->Destroy();

		glfwTerminate();

		s_Application = nullptr;
	}

	void Application::Run()
	{
		m_Running = true;

		float lastTime = GetTime();

		// Main Application loop
		while (m_Running)
		{
			glfwPollEvents();

			if (m_Window->ShouldClose())
			{
				Stop();
				break;
			}

			float currentTime = GetTime();
			float timestep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
			lastTime = currentTime;

			// Main layer update here
			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
				layer->OnUpdate(timestep);

			// NOTE: rendering can be done elsewhere (eg. render thread)
			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
				layer->OnRender();

			m_Window->Update();
		}
	}

	void Application::Stop()
	{
		m_Running = false;
	}

	glm::vec2 Application::GetFramebufferSize() const
	{
		return m_Window->GetFramebufferSize();
	}

	Application& Application::Get()
	{
		assert(s_Application);
		return *s_Application;
	}

	float Application::GetTime()
	{
		return (float)glfwGetTime();
	}

}