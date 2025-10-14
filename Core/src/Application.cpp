#include "Core/Application.h"

namespace Core
{
	static Application* s_Application = nullptr;

	Application::Application(const ApplicationSpecification& spec)
		: m_Specification(spec)
	{
		s_Application = this;

		glfwInit();

		if (m_Specification.WindowSpec.Title.empty())
			m_Specification.WindowSpec.Title = m_Specification.Name;

		m_Window = std::make_shared<Window>(m_Specification.WindowSpec);
		m_Window->Create();
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

		float lastFrameTime = GetTime();

		while(m_Running)
		{
			glfwPollEvents();

			if (m_Window->ShouldClose())
			{
				Stop();
				break;
			}

			float currentFrameTime = GetTime();
			float timeStep = glm::clamp(currentFrameTime - lastFrameTime, 0.001f, 0.1f);

			// Foreach layer: Update and Render

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
		return *s_Application;
	}

	float Application::GetTime()
	{
		return glfwGetTime();
	}
}