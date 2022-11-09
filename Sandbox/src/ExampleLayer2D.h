#include<Orion.h>

#include"imgui/imgui.h"
#include"glm/gtc/type_ptr.hpp"
#include"glm/gtx/string_cast.hpp"
#include"test/Skeleton.h"




class ExampleLayer2D : public Orion::Layer
{
public:
	ExampleLayer2D() : Layer("Example") {}
	bool skeletState = false;
	void Init() override
	{
		Orion::Renderer2D::Init();

		m_Camera = Orion::CreateShared<Orion::OrthographicCamera>(glm::vec3(0.0, 0.0f, 0.0f));

		Orion::CamerasController::AddCamera("PrimaryCamera", m_Camera);


		m_TextureJoker = Orion::Texture2D::Create("assets/textures/joker.png");
		m_TextureJokersFace = Orion::Texture2D::Create("assets/textures/jokersFace.png");
		m_Chess = Orion::Texture2D::Create("assets/textures/Board.png");
		m_SpriteSheet = Orion::Texture2D::Create("assets/textures/PathAndObjects.png");

		m_SkeletonSheet = Orion::Texture2D::Create("assets/textures/SkeletonSheet.png");



		m_AnimLibrary->CreateFromGrid("SkeletonIdle", m_SkeletonSheet, 10, glm::uvec2(10, 5), glm::uvec2(0, 4));
		m_AnimLibrary->CreateFromGrid("SkeletonWhoa", m_SkeletonSheet, 10, glm::uvec2(10, 5), glm::uvec2(0, 3));
		m_AnimLibrary->CreateFromGrid("SkeletonWalk", m_SkeletonSheet, 10, glm::uvec2(10, 5), glm::uvec2(0, 2));
		m_AnimLibrary->CreateFromGrid("SkeletonAttack", m_SkeletonSheet, 10, glm::uvec2(10, 5), glm::uvec2(0, 1));
		m_AnimLibrary->CreateFromGrid("SkeletonDeath", m_SkeletonSheet, 10, glm::uvec2(10, 5), glm::uvec2(0, 0));





		//m_StateMachine.SetStateFunction( ORI_BIND_ANIMATION_FN(SkeletonStatePattern));
		auto lib = m_AnimLibrary->GetMap();
		//Orion::AnimationRenderer2D::Create(m_SkeletonSheet, (uint32_t)10, glm::uvec2(10, 5));


		//Test::Skeleton Skeleton(glm::vec3(0.f), glm::vec2(1.f), m_Color);

		m_Skeleton = Orion::CreateShared<Test::Skeleton>(glm::vec3(0.f), glm::vec2(1.0f), m_Color);

	}

	void Update(Orion::Timestep deltaTime) override
	{
		ORI_PROFILE_FUNCTION();

		Orion::RenderCommand::SetClearColor(glm::vec4(0.850f, 0.796f, 0.937f, 1.0f));
		Orion::RenderCommand::Clear(ORI_CLEAR_COLOR | ORI_CLEAR_DEPTH | ORI_CLEAR_STENCIL);
		Orion::CamerasController::Update(deltaTime);



		Orion::Renderer2D::ResetStats();



		static float rotation = 0;
		rotation += deltaTime * 100.f;


		Orion::Renderer2D::BeginScene(Orion::CamerasController::GetActiveCamera());

		Orion::Renderer2D::DrawQuad(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec2(0.5f, 0.5f), glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
		for (size_t i = 0; i < 300; i++)
		{
			for (size_t j = 0; j < 300; j++)
			{
				Orion::Renderer2D::DrawBorderedQuad(glm::vec3(i, j, 0), m_Skeleton->GetSize(), glm::vec4(0.2f, 0.8f, 0.2f, 1.0f));
			}
		}

		m_Skeleton->Update(deltaTime, m_Color);






		Orion::Renderer2D::EndScene();




	}


	void OnEvent(Orion::Event& event) override
	{

		Orion::CamerasController::OnEvent(event);
		m_Dispatcher = Orion::EventDispatcher::CreateDispatcher(event);
		m_Dispatcher->Dispatch<Orion::KeyPressedEvent>(ORI_BIND_EVENT_FN(ExampleLayer2D::OnKeyPressed));
		m_Dispatcher->Dispatch<Orion::MouseMovedEvent>(ORI_BIND_EVENT_FN(ExampleLayer2D::OnMouseMoved));
	}

	bool OnMouseMoved(Orion::MouseMovedEvent e)
	{
		/*auto& cam = Orion::CamerasController::GetActiveCamera();
		float mouseX = e.GetX();
		float mouseY = e.GetY();
		ORI_CORE_INFO("Viewport: {0}", e);
		float x = (2.0f * mouseX) / 1200 - 1.0f;
		float y = 1.0f - (2.0f * mouseY) / 720;
		float z = 1.0f;

		glm::vec3 ray_nds = glm::vec3(x, y, z);

		std::cout << "NDS: " << glm::to_string(ray_nds) << std::endl;


		glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);


		std::cout << "CLIP: " << glm::to_string(ray_clip) << std::endl;


		glm::vec4 ray_eye = glm::inverse(cam->GetProjectionMatrix()) * ray_clip;


		std::cout << "EYE: " << glm::to_string(ray_eye) << std::endl;

		glm::vec3 ray_wor = glm::vec3((inverse(cam->GetViewMatrix()) * glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0f)));

		std::cout << "WORLD: " << glm::to_string(ray_eye) << std::endl;*/


		return false;
	}
	bool OnKeyPressed(Orion::KeyPressedEvent e)
	{
		if (e.GetKeyCode() == ORI_KEY_1)
		{
			Orion::CamerasController::SetActiveCamera("PrimaryCamera");
		}
		if (e.GetKeyCode() == ORI_KEY_2)
		{
			Orion::CamerasController::SetActiveCamera("SecondaryCamera");
		}



		return false;
	}






	virtual void OnImGuiRender(Orion::Timestep ts) override
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));

		auto& stats = Orion::Renderer2D::GetStats();


		ImGui::Text("FPS: %f", ts.GetFPS());
		ImGui::Text("DrawCalls: %d", stats.GetTotalDrawCalls());
		ImGui::Text("Quads: %d", stats.GetTotalQuadCount());
		ImGui::Text("QuadIndicies: %d", stats.GetTotalQuadIndexCount());
		ImGui::Text("Lines: %d", stats.GetTotalLineCount());
		ImGui::Text("Verticies: %d", stats.GetTotalVertexCount());



		ImGui::End();

	}
private:
	Orion::Shared<Test::Skeleton> m_Skeleton;
	Orion::Shared<Orion::AnimationLibrary> m_AnimLibrary;
	Orion::Shared<Orion::Texture2D> m_TextureJoker, m_TextureJokersFace, m_Chess, m_SpriteSheet, m_SkeletonSheet;
	Orion::Shared<Orion::EventDispatcher> m_Dispatcher;
	Orion::Shared<Orion::OrthographicCamera> m_Camera;
	glm::vec4 m_Color{ 0.842f, 0.523f, 0.768f, 1.0f };
	glm::vec3 m_Position{ 0,0,0 };
	glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
	uint32_t m_Counter;
};