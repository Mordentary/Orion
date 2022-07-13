#include<Orion.h>

#include"imgui/imgui.h"
#include"glm/gtc/type_ptr.hpp"
#include"Platform/OpenGL/OpenGLShader.h"
class ExampleLayer : public Orion::Layer
{
public:
	ExampleLayer() : Layer("Example"){}

	void Init() override
	{

		m_Camera = std::make_unique<Orion::OrthographicCamera>(-1.6, 1.6, -0.9, 0.9);
	

		float vertices[4 * 6] =
		{
			-0.5f, -0.5f,  0.0f,  0.3f, 0.4f,  0.0f,
			 0.5f, -0.5f,  0.0f,  0.7f, 0.11f, 0.61f,
			 -0.5f,  0.5f,  0.0f,  0.2f, 0.7f,  0.5f,
			  0.5f,  0.5f,  0.0f,  0.2f, 0.7f,  0.5f
		};

		uint32_t indices[3] = { 0,1,2 };
		uint32_t indicesQuad[6] = { 0, 1, 2, 2, 3, 1 };

		m_VertexArray = Orion::VertexArray::Create();
		m_VertexArray->Bind();

		m_VertexBuffer = Orion::VertexBuffer::Create(vertices, sizeof(vertices));
		Orion::BufferLayout layoutTriangle =
		{
			{Orion::ShaderDataType::Float3, "a_Position"},
			{Orion::ShaderDataType::Float3, "a_Color"},
		};

		m_VertexBuffer->SetLayout(layoutTriangle);
		m_IndexBuffer = Orion::IndexBuffer::Create(indices, std::size(indices));

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		m_VertexArray->Unbind();
		

		m_VertexArrayQuad = Orion::VertexArray::Create();
		m_VertexArrayQuad->Bind();

		m_IndexBufferQuad = Orion::IndexBuffer::Create(indicesQuad, std::size(indicesQuad));

		m_VertexArrayQuad->AddVertexBuffer(m_VertexBuffer);
		m_VertexArrayQuad->SetIndexBuffer(m_IndexBufferQuad);


		m_Camera->SetPosition({ 0.0f,0.0f, 0.0f });
		m_Shader = Orion::Shader::Create("assets/shaders/Triangle.glsl");
		m_ShaderQuad = Orion::Shader::Create("assets/shaders/Quad.glsl");
	}
	void OnUpdate(Orion::Timestep deltaTime) override
	{
		Orion::RenderCommand::SetClearColor(glm::vec4(0.850f, 0.796f, 0.937f, 1.0f));
		Orion::RenderCommand::Clear();

		CameraMove(0.1f * deltaTime);
	

		Orion::Renderer::BeginScene(m_Camera);
		m_Shader->Bind();
		Orion::Renderer::Submit(m_Shader, m_VertexArray, m_Model);
		m_ShaderQuad->Bind();
		std::dynamic_pointer_cast<Orion::OpenGLShader>(m_ShaderQuad)->UploadUniformFloat3("u_Color", m_Color);
		Orion::Renderer::Submit(m_ShaderQuad, m_VertexArrayQuad, glm::mat4(1.0f));



		Orion::Renderer::EndScene();
	}
	void OnEvent(Orion::Event& event) override
	{
		m_Dispatcher = Orion::EventDispatcher::CreateDispatcher(event);

		m_Dispatcher->Dispatch<Orion::KeyPressedEvent>(ORI_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));
	}

	bool OnKeyPressed(Orion::KeyPressedEvent e)
	{
		
		return false;
		
	}
	void CameraMove(float&& speed) 
	{
		if (Orion::Input::IsKeyPressed(ORI_KEY_W))
		{
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(0.0f, speed, 0.0f));
			
		}
		if (Orion::Input::IsKeyPressed(ORI_KEY_S))
		{
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(0.0f, -speed, 0.0f));
		
		}
		if (Orion::Input::IsKeyPressed(ORI_KEY_D))
		{
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(speed, 0.0f, 0.0f));
		
		}
		if (Orion::Input::IsKeyPressed(ORI_KEY_A))
		{
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::vec3(-speed, 0.0f, 0.0f));
			
		}
		if (Orion::Input::IsKeyPressed(ORI_KEY_UP))
		{
			m_Model = glm::translate(m_Model, glm::vec3(0.0f, speed, 0.0f));
		}
		if (Orion::Input::IsKeyPressed(ORI_KEY_DOWN))
		{
			m_Model = glm::translate(m_Model, glm::vec3(0.0f, -speed, 0.0f));
		}
		if (Orion::Input::IsKeyPressed(ORI_KEY_LEFT))
		{
			m_Model = glm::translate(m_Model, glm::vec3(-speed, 0.0f, 0.0f));
		}
		if (Orion::Input::IsKeyPressed(ORI_KEY_RIGHT))
		{
			m_Model = glm::translate(m_Model, glm::vec3(speed, 0.0f, 0.0f));
		}
	
	}

	virtual void OnImGuiRender() override 
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
		ImGui::End();
		
	}
private:
	Orion::Ref<Orion::Shader> m_ShaderQuad;
	Orion::Ref<Orion::IndexBuffer> m_IndexBufferQuad;
	Orion::Ref<Orion::VertexArray> m_VertexArrayQuad;

	Orion::Ref<Orion::Shader> m_Shader;
	Orion::Ref<Orion::VertexBuffer> m_VertexBuffer;
	Orion::Ref<Orion::IndexBuffer> m_IndexBuffer;
	Orion::Ref<Orion::VertexArray> m_VertexArray;
	Orion::Ref<Orion::EventDispatcher> m_Dispatcher;
	Orion::Ref<Orion::OrthographicCamera> m_Camera;
	glm::vec3 m_Color { 1.0f, 1.0f, 1.0f};
	glm::mat4 m_Model = glm::mat4(1.0f);
};

class Sandbox : public Orion::Application 
{
public: 
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
		//PushOverlay(new Orion::ImGuiLayer());
	}
	~Sandbox() 
	{
	
	}
};

Orion::Application* Orion::CreateApplication()
{
	return new Sandbox();
}