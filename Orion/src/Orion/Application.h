#pragma once

#include"Core.h"

#include"Window.h"
#include"Events/Event.h"
#include"Orion/Layers/LayerStack.h"
#include "Orion/Events/ApplicationEvent.h"

namespace Orion {

	class ORION_API Application
	{
		public: 
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
		
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);


		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }
	private: 

		bool OnWindowClose(WindowCloseEvent& e);
		bool m_Running = true;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;

	};
	//Defined on a client side
	Application* CreateApplication();
}

