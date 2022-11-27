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
		m_Camera = Orion::CreateShared<Orion::PerspectiveCamera>(glm::vec3(0.0, 0.0f, 2.0f), glm::vec3(0.0f));
		auto m_Camera2 = Orion::CreateShared<Orion::OrthographicCamera>(glm::vec3(0.0, 0.0f, 0.0f));

		Orion::CamerasController::AddCamera("OrthoCamera", m_Camera2);
		Orion::CamerasController::AddCamera("PerspectiveCamera", m_Camera);
		m_DiffuseMap = Orion::Texture2D::Create("assets/textures/container.png");
		m_SpecularMap = Orion::Texture2D::Create("assets/textures/container_specular.png");
		m_SkyTexture = Orion::Texture2D::Create("E:/Development/Orion/Sandbox/assets/textures/Cubemap/top.jpg");



		m_ModelCat = Orion::CreateShared<Orion::Model>("assets/models/Cat/Cat.obj");
		m_ModelPlatform = Orion::CreateShared<Orion::Model>("assets/models/Platform.FBX");
		m_ModelLamp = Orion::CreateShared<Orion::Model>("assets/models/Lamp/source/SM_Lamp_01a.FBX");
		m_ModelTree = Orion::CreateShared<Orion::Model>("assets/models/Tree/Tree.obj");
		m_ModelCar = Orion::CreateShared<Orion::Model>("assets/models/Car/source/hw6.obj");
		m_ModelDragon = Orion::CreateShared<Orion::Model>("assets/models/Pig/source/model.dae");
		//m_ModelScene = Orion::CreateShared<Orion::Model>("assets/models/Scene/sponza/NewSponza_Main_Yup_002.fbx");


		//m_ModelScene->SetScale(glm::vec3(5.f));

		m_ModelCat->SetPosition(glm::vec3(0.0, 0.0, 1.0));


		m_ModelPlatform->SetScale(glm::vec3(50.0f, 10.0f, 50.0f));
		m_ModelPlatform->SetPosition(glm::vec3(0.0, -4.0f, 5.0));

		m_ModelTree->SetScale(glm::vec3(20.0f, 15.0f, 20.0f));
		m_ModelTree->SetPosition(glm::vec3(-0.3, -4.0f, 10.5));

		m_ModelCar->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
		m_ModelCar->SetPosition(glm::vec3(-0.7, -0.7, -5.2));


		m_ModelDragon->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
		m_ModelDragon->SetPosition(glm::vec3(-0.6, -0.6, -6.2));



		//m_PointLight2 = Orion::CreateShared<Orion::PointLight>();
		//m_PointLight3 = Orion::CreateShared<Orion::PointLight>();

		m_PointLight = Orion::CreateShared<Orion::PointLight>();
		m_SpotLight = Orion::CreateShared<Orion::SpotLight>(m_ModelLamp);
		m_DirLight = Orion::CreateShared<Orion::DirectionalLight>();

		m_SpotLight->GetGeneralLightProp().Direction = glm::vec3(0.0f, -1.0f, 0.0f);
		m_SpotLight->GetGeneralLightProp().Position = glm::vec3(0.0f, 3.0f, 0.0f);



		Orion::Renderer::AddSphereToScene(glm::mat4(1.0f), { nullptr,nullptr,nullptr,0 });
		Orion::Renderer::AddCubeToScene(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.f)), { nullptr,nullptr,nullptr, 2.f });



		Orion::Renderer::AddModelToScene(m_ModelCat);
		Orion::Renderer::AddModelToScene(m_ModelPlatform);
		Orion::Renderer::AddModelToScene(m_ModelTree);
		Orion::Renderer::AddModelToScene(m_ModelCar);
		Orion::Renderer::AddModelToScene(m_ModelDragon);


		Orion::Renderer::AddLightToScene(m_SpotLight);
		Orion::Renderer::AddLightToScene(m_PointLight);
		Orion::Renderer::AddLightToScene(m_DirLight);


		std::vector<std::string> cubeMapPaths
		{
				"E:/Development/Orion/Sandbox/assets/textures/Cubemap/right.jpg",
				"E:/Development/Orion/Sandbox/assets/textures/Cubemap/left.jpg",
				"E:/Development/Orion/Sandbox/assets/textures/Cubemap/top.jpg",
				"E:/Development/Orion/Sandbox/assets/textures/Cubemap/bottom.jpg",
				"E:/Development/Orion/Sandbox/assets/textures/Cubemap/front.jpg",
				"E:/Development/Orion/Sandbox/assets/textures/Cubemap/back.jpg"
		};

		m_CubeMap = Orion::Texture2D::CreateCubemap(cubeMapPaths);

		Orion::FramebufferSpecification specFB;
		specFB.Width = Orion::Application::Get().GetWindow().GetWidth();
		specFB.Height = Orion::Application::Get().GetWindow().GetHeight();
		specFB.Samples = 4;
		specFB.ColorAttachments = 2;


		m_FramebufferMS = Orion::Framebuffer::Create(specFB);
		//m_Framebuffer_Refra = Orion::Framebuffer::Create(specFB);

		specFB.Samples = 1;
		specFB.ColorAttachments = 1;
		specFB.sRGB_ColorAttach = true;


		m_FinalFramebuffer = Orion::Framebuffer::Create(specFB);


		//Orion::Renderer::SetSceneCubemap(m_PointLight->GetShadowmap());
		Orion::Renderer::AddSceneCubemap(m_CubeMap);
	}

	void Update(Orion::Timestep deltaTime) override
	{
		ORI_PROFILE_FUNCTION();

		Orion::CamerasController::Update(deltaTime);
		Orion::Renderer::UpdateLightSettings(m_LightSettings.x, m_LightSettings.y);


		glm::vec3 lightPos{};
		float time = Orion::CurrentTime::GetCurrentTimeInSec();

		lightPos.x = sin(time) * 2.0f;
		lightPos.y = 4.0f;
		lightPos.z = cos(time) * 2.0f;

		m_PointLight->GetGeneralLightProp().Position = lightPos / 3.f;
		m_PointLight->GetGeneralLightProp().DiffuseLightColor = m_Color;
		m_PointLight->GetGeneralLightProp().SpecularLightColor = m_Color / 2.f;

		//ORI_INFO("ScenePos {0}", glm::to_string(m_ModelScene->GetPosition()));
		//ORI_INFO("SceneSize {0}", glm::to_string(m_ModelScene->GetScale()));


		m_ModelCar->SetPosition(m_ModelCar->GetPosition());


		//m_PointLight2->GetGeneralLightProp().Position = lightPos * 1.f;
		//m_PointLight2->GetGeneralLightProp().DiffuseLightColor = glm::vec3(0.2f, m_Color.g, 0.2f);
		//m_PointLight2->GetGeneralLightProp().SpecularLightColor = glm::vec3(0.2f, m_Color.g, 0.2f)  / 2.f;
		//
		//m_PointLight3->GetGeneralLightProp().Position = lightPos * 4.f;
		//m_PointLight3->GetGeneralLightProp().DiffuseLightColor = glm::vec3(0.2f, 0.2f, m_Color.b);
		//m_PointLight3->GetGeneralLightProp().SpecularLightColor = glm::vec3(0.2f, m_Color.g, 0.2f)  / 2.f;


		m_SpotLight->GetGeneralLightProp().Position = glm::vec3(cos(time), 3.0f, sin(time));

		m_DirLight->GetGeneralLightProp().Direction = m_SunDirection;
		m_DirLight->GetGeneralLightProp().DiffuseLightColor = m_SunColor;
		m_DirLight->GetGeneralLightProp().SpecularLightColor = m_SunColor / 2.f;



		///Orion::Renderer::BeginScene(Orion::CamerasController::GetActiveCamera(), m_FramebufferMS, [this]() {Render(); });
		///
		///Render();
		///
		///Orion::Renderer::EndScene();
		///
		///Orion::Renderer::PostProcessing(m_FinalFramebuffer, m_PostProcessSpec);
		///
		///m_FinalFramebuffer->BlitToDefaultBuffer();

		

	}
	void Render() override
	{


		Orion::Renderer::DrawScene();

		Orion::CamerasController::GetActiveCamera()->Raycast(Orion::Input::GetMouseX(), Orion::Input::GetMouseY()).DebugDraw();


	}


	void OnEvent(Orion::Event& event) override
	{
		Orion::CamerasController::OnEvent(event);
		m_Dispatcher = Orion::EventDispatcher::CreateDispatcher(event);
		m_Dispatcher->Dispatch<Orion::KeyPressedEvent>(ORI_BIND_EVENT_FN(OnKeyPressed));
		m_Dispatcher->Dispatch<Orion::MouseMovedEvent>(ORI_BIND_EVENT_FN(OnMouseMoved));


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

private:
	Orion::Shared<Orion::DummyCamera> m_Camera, m_LightDirCam;
	Orion::Shared<Orion::EventDispatcher> m_Dispatcher;
	glm::vec4 m_Color{ 0.842f, 0.523f, 0.768f, 1.0f }, m_SunColor{ 0.7f };
	glm::vec3 m_Position{ 0,0,0 };
	glm::vec3 m_SunDirection{ 0.1f,-1.f,0.1f };

	Orion::Renderer::PostProcessSpec m_PostProcessSpec{};
	glm::vec2 m_LightSettings{ 2.0f,0.100f };
	glm::vec2 m_ViewportSize;
	glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
	Orion::Shared<Orion::Framebuffer> m_FramebufferMS, m_FinalFramebuffer, m_Framebuffer_Refra;
	Orion::Shared<Orion::Model> m_ModelCat, m_ModelPlatform, m_ModelLamp, m_ModelTree, m_ModelCar, m_ModelDragon, m_ModelScene;
	Orion::Shared<Orion::LightSource> m_SpotLight, m_DirLight, m_PointLight, m_PointLight2, m_PointLight3;
	Orion::Shared<Orion::Texture2D> m_DiffuseMap, m_SpecularMap, m_CubeMap, m_SkyTexture;
};
