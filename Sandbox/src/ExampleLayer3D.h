#include<Orion.h>

#include"imgui/imgui.h"
#include"glm/gtc/type_ptr.hpp"
#include"glm/gtx/string_cast.hpp"






class ExampleLayer3D : public Orion::Layer
{
public:
	ExampleLayer3D() : Layer("Example") {}
	void Init() override
	{
		Orion::Renderer::Init();

		m_Camera = Orion::CreateShared<Orion::PerspectiveCamera>(glm::vec3(0.0,0.0f,3.0f),glm::vec3(0.0f));
		auto m_Camera2 = Orion::CreateShared<Orion::OrthographicCamera>(glm::vec3(0.0, 0.0f, 0.0f));


		Orion::CamerasController::AddCamera("PerspectiveCamera", m_Camera);
		Orion::CamerasController::AddCamera("OrthoCamera", m_Camera2);
		m_DiffuseMap = Orion::Texture2D::Create("assets/textures/container.png");
		m_SpecularMap = Orion::Texture2D::Create("assets/textures/container_specular.png");

		

		m_SpotLight = Orion::CreateShared<Orion::SpotLight>(); 
		m_PointLight = Orion::CreateShared<Orion::PointLight>();
		m_DirLight = Orion::CreateShared<Orion::DirectionalLight>();

		m_SpotLight->GetLightProperties().Direction = glm::vec3(0.0f, -1.0f, 0.0f);
		m_SpotLight->GetLightProperties().Position= glm::vec3(0.0f, 1.0f, 0.0f);

		Orion::Renderer::AddLight(m_SpotLight);
		Orion::Renderer::AddLight(m_PointLight);
		Orion::Renderer::AddLight(m_DirLight);
		m_ModelCat = Orion::CreateShared<Orion::Model>("assets/models/Cat/Cat.obj");

	}

	void OnUpdate(Orion::Timestep deltaTime) override
	{
		ORI_PROFILE_FUNCTION();

		Orion::RenderCommand::SetClearColor(glm::vec4(0.850f, 0.796f, 0.937f, 1.0f));
		Orion::RenderCommand::Clear();
		Orion::CamerasController::OnUpdate(deltaTime);

		//Orion::Renderer2D::ResetStats();

		
		static float rotation = 0;
	//	rotation += deltaTime * 100.f;
		ORI_INFO("Pos: {0}", glm::to_string(Orion::CamerasController::GetActiveCamera()->GetPosition()));



		glm::vec3 lightPos;
		float time = Orion::CurrentTime::GetCurrentTimeInSec();
		lightPos.x = sin(time) * 1.0f;
		lightPos.y = 1.f;
		lightPos.z = cos(time) * 1.0f;
		m_SpotLight->GetLightProperties().Direction = glm::vec3(cos(time)/4, -1.0f, sin(time) / 4);

		glm::mat4 lightMatrix = glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		m_PointLight->GetLightProperties().Position = lightPos;
		m_DirLight->GetLightProperties().Direction = m_SunDirection;

		Orion::Renderer::BeginScene(Orion::CamerasController::GetActiveCamera());

		Orion::Material mat =
		{
		//	m_DiffuseMap, m_SpecularMap, 16.f
		};



		Orion::Renderer::DrawModel(glm::mat4(1.0f), m_ModelCat);

	//	Orion::Renderer::DrawCube(lightMatrix, mat);
	//	Orion::Renderer::DrawCube(glm::mat4(1.0f),mat);


		Orion::Renderer::EndScene();

	}


	void OnEvent(Orion::Event& event) override
	{
		Orion::CamerasController::OnEvent(event);
		m_Dispatcher = Orion::EventDispatcher::CreateDispatcher(event);
		m_Dispatcher->Dispatch<Orion::KeyPressedEvent>(ORI_BIND_EVENT_FN(ExampleLayer3D::OnKeyPressed));
		m_Dispatcher->Dispatch<Orion::MouseMovedEvent>(ORI_BIND_EVENT_FN(ExampleLayer3D::OnMouseMoved));


		Orion::CamerasController::OnEvent(event);
	}

	bool OnMouseMoved(Orion::MouseMovedEvent e)
	{

		return false;
	}
	bool OnKeyPressed(Orion::KeyPressedEvent e)
	{
		if (e.GetKeyCode() == ORI_KEY_1)
		{
			Orion::CamerasController::SetActiveCamera("OrthoCamera");
		}
		if (e.GetKeyCode() == ORI_KEY_2)
		{
			Orion::CamerasController::SetActiveCamera("PerspectiveCamera");
		}

		return false;
	}






	virtual void OnImGuiRender(Orion::Timestep ts) override
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
		ImGui::SliderFloat3("DirLight ", glm::value_ptr(m_SunDirection), -10.0f, 10.0f);

		/*auto& stats = Orion::Renderer::GetStats();


		ImGui::Text("FPS: %f", ts.GetFPS());
		ImGui::Text("DrawCalls: %d", stats.GetTotalDrawCalls());
		ImGui::Text("Quads: %d", stats.GetTotalQuadCount());
		ImGui::Text("QuadIndicies: %d", stats.GetTotalQuadIndexCount());
		ImGui::Text("Lines: %d", stats.GetTotalLineCount());
		ImGui::Text("Verticies: %d", stats.GetTotalVertexCount());*/



		ImGui::End();

	}
private:
	Orion::Shared<Orion::PerspectiveCamera> m_Camera;
	Orion::Shared<Orion::EventDispatcher> m_Dispatcher;
	glm::vec4 m_Color{ 0.842f, 0.523f, 0.768f, 1.0f };
	glm::vec3 m_Position{ 0,0,0 };
	glm::vec3 m_SunDirection{ 0,0,0 };

	glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
	Orion::Shared<Orion::Model> m_ModelCat;
	Orion::Shared<Orion::LightSource> m_SpotLight, m_DirLight, m_PointLight;
	Orion::Shared<Orion::Texture2D> m_DiffuseMap, m_SpecularMap;
};