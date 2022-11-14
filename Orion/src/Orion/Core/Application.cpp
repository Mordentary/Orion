#include"oripch.h"
#include "Application.h"



#include"Orion/Core/Input.h"
#include"Orion/Renderer/GraphicsRendering/Renderer.h"
#include"Orion/Renderer/GraphicsRendering/AnimationRenderer2D.h"
#include"Orion/Core/TimeHelper.h"

namespace Orion 
{

	//#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		ORI_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;
		
		m_Window = Scoped<Window>(Window::Create(WindowProps(name)));
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
		Timestep deltaTime(0.16);
		while (m_Running)
		{
			timer.Start();
			deltaTime = timer.GetTimeSeconds();
			//ORI_CORE_INFO("DeltaTime: {0}",deltaTime);

			if (!m_Minimized) 
			{
				AnimationRenderer2D::SetDelta(deltaTime);
				for (Layer* layer : m_LayerStack)
					layer->Update(deltaTime);
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
		RenderCommand::SetViewport(0,0,e.GetWidth(),e.GetHeight());
		return false;
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}