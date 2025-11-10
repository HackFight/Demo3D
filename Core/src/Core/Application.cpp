#include "Core/Application.h"

// std
#include <memory>

// Core
#include "RendererAPI/GLUtils.h"

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

		double lastFrameTime = GetTime();

		while(m_Running)
		{
			glfwPollEvents();

			if (m_Window->ShouldClose())
			{
				Stop();
				break;
			}

			double currentFrameTime = GetTime();
			double timeStep = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

			// Foreach layer: Update and Render
			for (const std::unique_ptr<Layer>& layer : m_LayerStack) { layer->OnUpdate(timeStep); }

			for (const std::unique_ptr<Layer>& layer : m_LayerStack) { layer->OnRender(); }
			
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

	std::shared_ptr<Window> Application::GetWindow()
	{
		return m_Window;
	}

	Application& Application::Get()
	{
		return *s_Application;
	}

	double Application::GetTime()
	{
		return glfwGetTime();
	}
}