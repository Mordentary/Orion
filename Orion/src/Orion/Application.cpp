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


		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		float vertices[3 * 6] =
		{
			-0.5f, -0.5f,  0.0f,  0.3f, 0.4f,  0.0f,
			 0.5f, -0.5f,  0.0f,  0.7f, 0.11f, 0.61f,
			 0.0f,  0.5f,  0.0f,  0.2f, 0.7f,  0.5f
		};


		GLuint indices[3] = {0,1,2};


		m_VertexArray = VertexArray::Create();
		m_VertexArray->Bind();


		m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float3, "a_Color"}, 
			

		};
		m_VertexBuffer->SetLayout(layout);


		m_IndexBuffer = IndexBuffer::Create(indices, std::size(indices));

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

	

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Color;
		

			out vec3 v_color;

			void main()
			{
				v_color = a_Color;
				gl_Position = vec4(a_Position,1.0);
			}
		)";

		std::string fragSrc = R"(
			#version 330 core
			out vec4 color;

			in vec3 v_color;

			void main()
			{
				color = vec4(v_color,1.0);
			}
		)";



		m_Shader = Shader::Create(vertexSrc, fragSrc);

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
			glClearColor(0.850f, 0.796f, 0.937f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);



			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
		

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}