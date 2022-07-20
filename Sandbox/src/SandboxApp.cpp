#include<Orion.h>

#include"imgui/imgui.h"
#include"glm/gtc/type_ptr.hpp"
#include"Platform/OpenGL/OpenGLShader.h"
#include"Orion/Core/Camera2D/CamerasController2D.h"
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
		float verticesTex[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		uint32_t indices[3] = { 0,1,2 };
		uint32_t indicesQuad[6] = { 0, 1, 2, 2, 3, 0 };

		m_VertexArray = Orion::VertexArray::Create();
		m_VertexArray->Bind();

		m_VertexBuffer = Orion::VertexBuffer::Create(vertices, sizeof(vertices));
		Orion::BufferLayout layoutTriangle =
		{
			{Orion::ShaderDataType::Float3, "a_Position"},
			{Orion::ShaderDataType::Float3, "a_Color"}
		};

		m_VertexBuffer->SetLayout(layoutTriangle);
		m_IndexBuffer = Orion::IndexBuffer::Create(indices, std::size(indices));

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		m_VertexArray->Unbind();
		



		m_VertexArrayQuad = Orion::VertexArray::Create();
		m_VertexArrayQuad->Bind();

		m_VertexBufferQuad = Orion::VertexBuffer::Create(verticesTex, sizeof(verticesTex));
		Orion::BufferLayout layoutQuad =
		{
			{Orion::ShaderDataType::Float3, "a_Position"},
			{Orion::ShaderDataType::Float2, "a_Texture"}
		};

		m_VertexBufferQuad->SetLayout(layoutQuad);
		m_IndexBufferQuad = Orion::IndexBuffer::Create(indicesQuad, std::size(indicesQuad));

		m_VertexArrayQuad->AddVertexBuffer(m_VertexBufferQuad);
		m_VertexArrayQuad->SetIndexBuffer(m_IndexBufferQuad);




		m_Shader = m_ShaderLibrary.Load("assets/shaders/Triangle.glsl");
		m_ShaderTexture = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		Orion::CamerasController2D::AddCamera("PrimaryCamera", m_Camera->GetAspectRatio());
		Orion::CamerasController2D::AddCamera("SecondaryCamera", m_Camera);


		m_Texture2D = Orion::Texture2D::Create("assets/textures/joker.png");




		std::dynamic_pointer_cast<Orion::OpenGLShader>(m_ShaderTexture)->Bind();
		std::dynamic_pointer_cast<Orion::OpenGLShader>(m_ShaderTexture)->UploadUniformInt("u_Texture", 0);

	}
	void OnUpdate(Orion::Timestep deltaTime) override
	{
		Orion::RenderCommand::SetClearColor(glm::vec4(0.850f, 0.796f, 0.937f, 1.0f));
		Orion::RenderCommand::Clear();
		Orion::CamerasController2D::OnUpdate(deltaTime);

	

		Orion::Renderer::BeginScene(Orion::CamerasController2D::GetActiveCamera());
	
		Orion::Renderer::Submit(m_Shader, m_VertexArray, m_Model);
		
	
		m_JokerModel = glm::rotate(m_JokerModel, 0.5f * deltaTime, glm::vec3(0.0, 1.0, 0.0));
		m_Texture2D->Bind();
		Orion::Renderer::Submit(m_ShaderTexture, m_VertexArrayQuad, m_JokerModel);



		Orion::Renderer::EndScene();
	}
	void OnEvent(Orion::Event& event) override
	{
		Orion::CamerasController2D::OnEvent(event);
		m_Dispatcher = Orion::EventDispatcher::CreateDispatcher(event);
		m_Dispatcher->Dispatch<Orion::KeyPressedEvent>(ORI_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));
	}

	bool OnKeyPressed(Orion::KeyPressedEvent e)
	{
		if (e.GetKeyCode() == ORI_KEY_1)
		{
			Orion::CamerasController2D::SetActiveCamera("PrimaryCamera");
		}
		if (e.GetKeyCode() == ORI_KEY_2) 
		{
			Orion::CamerasController2D::SetActiveCamera("SecondaryCamera");
		}
		return false;
		
	}

	virtual void OnImGuiRender() override 
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
		ImGui::End();
		
	}
private:
	
	Orion::Shared<Orion::Shader> m_ShaderTexture;
	Orion::Shared<Orion::VertexArray> m_VertexArrayQuad;
	Orion::Shared<Orion::IndexBuffer> m_IndexBufferQuad;
	Orion::Shared<Orion::VertexBuffer> m_VertexBufferQuad;
	Orion::Shared<Orion::Texture2D> m_Texture2D;
	Orion::ShaderLibrary m_ShaderLibrary;
	glm::mat4 m_JokerModel = glm::mat4(1.0f);

	Orion::Shared<Orion::Shader> m_Shader;
	Orion::Shared<Orion::VertexBuffer> m_VertexBuffer;
	Orion::Shared<Orion::IndexBuffer> m_IndexBuffer;
	Orion::Shared<Orion::VertexArray> m_VertexArray;

	Orion::Shared<Orion::EventDispatcher> m_Dispatcher;
	Orion::Shared<Orion::OrthographicCamera> m_Camera;
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