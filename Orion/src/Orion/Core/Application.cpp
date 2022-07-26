#include"oripch.h"
#include "Application.h"



#include"Orion/Core/Input.h"
#include"Orion/Renderer/Renderer.h"
#include"Orion/Core/TimeHelper.h"

namespace Orion 
{

	//#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{
		ORI_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;
		
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(ORI_BIND_EVENT_FN(Application::OnEvent));


		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	
	};

	Application::~Application() 
	{

	};

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}


	void Application::OnEvent(Event& e)
	{
	
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(ORI_BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(ORI_BIND_EVENT_FN(OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled) { break; }
		}

	}
	
	void Application::Run() 
	{
		for (Layer* layer : m_LayerStack)
			layer->Init();
		
		Timer timer("DeltaTimer");
		
		while (m_Running)
		{
			timer.Start();
			Timestep deltaTime = timer.GetTimeSeconds();
			//ORI_CORE_INFO("DeltaTime: {0}",deltaTime);


			if (!m_Minimized) 
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(deltaTime);
			}
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender(deltaTime);
			m_ImGuiLayer->End();

			m_Window->OnUpdate();



			timer.End();
		}
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 1) {
			m_Minimized = true;  
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(),e.GetHeight());
		return false;
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}