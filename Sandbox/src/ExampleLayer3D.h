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

		Orion::Renderer::BeginScene(Orion::CamerasController::GetActiveCamera());


		Orion::Renderer::DrawCube(glm::mat4(1.0f));



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
	glm::mat4 m_Model = glm::mat4(1.0f);
};