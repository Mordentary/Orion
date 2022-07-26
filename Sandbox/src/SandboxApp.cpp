#include<Orion.h>

#include"imgui/imgui.h"
#include"glm/gtc/type_ptr.hpp"
#include"Orion/Core/Camera2D/CamerasController2D.h"
class ExampleLayer : public Orion::Layer
{
public:
	ExampleLayer() : Layer("Example"){}

	void Init() override
	{
		Orion::Renderer::Init();

		m_Camera = std::make_unique<Orion::OrthographicCamera>(-1.6, 1.6, -0.9, 0.9);

		Orion::CamerasController2D::AddCamera("PrimaryCamera", m_Camera->GetAspectRatio());
		Orion::CamerasController2D::AddCamera("SecondaryCamera", m_Camera);


		m_TextureJoker = Orion::Texture2D::Create("assets/textures/joker.png");
		m_TextureJokersFace = Orion::Texture2D::Create("assets/textures/jokersFace.png");
		m_Chess = Orion::Texture2D::Create("assets/textures/kek.png");
		

	}
	void OnUpdate(Orion::Timestep deltaTime) override
	{
		ORI_PROFILE_FUNCTION();

		Orion::RenderCommand::SetClearColor(glm::vec4(0.850f, 0.796f, 0.937f, 1.0f));
		Orion::RenderCommand::Clear();
		Orion::CamerasController2D::OnUpdate(deltaTime);

		Orion::Renderer2D::ResetStats();

	

		static float rotation = 0;
		rotation += deltaTime * 100.f;


		Orion::Renderer2D::BeginScene(Orion::CamerasController2D::GetActiveCamera());

			for (size_t i = 0; i < 100; i++)
			{
				for (size_t j = 0; j < 100; j++)
				{

					Orion::Renderer2D::DrawTexturedQuad(glm::vec2(i, j), glm::vec2(0.2, 0.2), m_TextureJokersFace, glm::vec4({ 0.842f, 0.523f, 0.768f, 1.0f }));
					Orion::Renderer2D::DrawQuad(glm::vec3(i, j, 0.0f), glm::vec2(0.49, 0.49), m_Color);
				

				}
			}
			/*for (size_t i = 0; i < 100; i++)
			{
				for (size_t j = 0; j < 100; j++)
				{
					Orion::Renderer2D::DrawTexturedQuad(glm::vec3(i + 0.6f, j + 0.6f, 0.0f), glm::vec2(0.4, 0.4), m_TextureJoker, m_Color);
				}
			}*/

		Orion::Renderer2D::EndScene();




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

	virtual void OnImGuiRender(Orion::Timestep ts) override
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));

		auto& stats = Orion::Renderer2D::GetStats();
		ImGui::Text("FPS: %f",ts.GetFPS());
		ImGui::Text("DrawCalls: %d", stats.GetTotalDrawCalls());
		ImGui::Text("Quads: %d", stats.GetTotalQuadCount());
		ImGui::Text("Vertcies: %d", stats.GetTotalVertexCount());
		ImGui::Text("Index: %d", stats.GetTotalIndexCount());

	
		 

		ImGui::End();
		
	}
private:
	
	Orion::ShaderLibrary m_ShaderLibrary;
	Orion::Shared<Orion::EventDispatcher> m_Dispatcher;
	Orion::Shared<Orion::OrthographicCamera> m_Camera;
	Orion::Shared<Orion::Texture2D> m_TextureJoker, m_TextureJokersFace, m_Chess;


	glm::vec4 m_Color { 0.842f, 0.523f, 0.768f, 1.0f};
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