#pragma once

#include"Core.h"

#include"Window.h"
#include"Orion/Layers/LayerStack.h"
#include"Events/Event.h"
#include "Orion/Events/ApplicationEvent.h"

#include"Orion/ImGui/ImGuiLayer.h"

#include"Orion/Renderer/Shader.h"
#include"Orion/Renderer/VertexArray.h"
#include"Orion/Renderer/Buffer.h"

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

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		bool m_Running = true;

	

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexArray> m_VertexArray;

	private:

		static Application* s_Instance;

	};
	//Defined on a client side
	Application* CreateApplication();
}

