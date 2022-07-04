#include"oripch.h"
#include "Application.h"

#include<Glad/glad.h>

#include"Orion/Input.h"

namespace Orion 
{

	//#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{
		ORI_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;
		
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback([this](Event& s){Application::OnEvent(s);});
	};

	Application::~Application() 
	{

	};

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}


	void Application::OnEvent(Event& e)
	{
	
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(ORI_BIND_EVENT_FN(OnWindowClose));
		

		//ORI_CORE_TRACE("{0}",e);


		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled) { break; }
		}

	}

	void Application::Run() 
	{
		
		while (m_Running)
		{
			glClearColor(0.850, 0.796, 0.937, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			ORI_CORE_TRACE("{0}", Input::IsKeyPressed(83));

			m_Window->OnUpdate();
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}