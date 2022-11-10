#include<Orion.h>

#include"imgui/imgui.h"
#include"glm/gtc/type_ptr.hpp"
#include"glm/gtx/string_cast.hpp"


namespace Orion {
	class EditLayer3D : public Orion::Layer
	{
	public:
		EditLayer3D() : Layer("Example") {}
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

			m_SpotLight = Orion::CreateShared<Orion::SpotLight>(m_ModelLamp);
			m_PointLight = Orion::CreateShared<Orion::PointLight>();
			//m_PointLight2 = Orion::CreateShared<Orion::PointLight>();
			//m_PointLight3 = Orion::CreateShared<Orion::PointLight>();

			m_DirLight = Orion::CreateShared<Orion::DirectionalLight>();

			m_SpotLight->GetLightProperties().Direction = glm::vec3(0.0f, -1.0f, 0.0f);
			m_SpotLight->GetLightProperties().Position = glm::vec3(0.0f, 3.0f, 0.0f);



			Orion::Renderer::AddLight(m_SpotLight);
			//Orion::Renderer::AddLight(m_PointLight);
			//Orion::Renderer::AddLight(m_DirLight);


			std::vector<std::string> cubeMapPaths
			{
					"E:/Development/Orion/Sandbox/assets/textures/Cubemap/right.jpg",
					"E:/Development/Orion/Sandbox/assets/textures/Cubemap/left.jpg",
					"E:/Development/Orion/Sandbox/assets/textures/Cubemap/top.jpg",
					"E:/Development/Orion/Sandbox/assets/textures/Cubemap/bottom.jpg",
					"E:/Development/Orion/Sandbox/assets/textures/Cubemap/front.jpg",
					"E:/Development/Orion/Sandbox/assets/textures/Cubemap/back.jpg"
			};

			m_CubeMap = Texture2D::CreateCubemap(cubeMapPaths);

			Orion::FramebufferSpecification specFB;
			specFB.Width = Orion::Application::Get().GetWindow().GetWidth();
			specFB.Height = Orion::Application::Get().GetWindow().GetHeight();
			specFB.Samples = 6;



			m_FramebufferMS = Orion::Framebuffer::Create(specFB);
			m_Framebuffer_Refra = Orion::Framebuffer::Create(specFB);

			specFB.Samples = 1;
			m_Framebuffer = Orion::Framebuffer::Create(specFB);

			specFB.Width = 4096;
			specFB.Height = 4096;
			specFB.OnlyDepthPass = true;

			m_ShadowMapDir = Orion::Framebuffer::Create(specFB);

			specFB.CubemapBuffer = true;
			//m_ShadowMapPoint = Orion::Framebuffer::Create(specFB);
			m_ShadowMapSpot = Orion::Framebuffer::Create(specFB);


			//Orion::Renderer::SetSceneCubemap(m_PointLight->GetShadowmap());
			Orion::Renderer::SetSceneCubemap(m_CubeMap);


			 m_LightDirCam = Orion::CreateShared<Orion::OrthographicCamera>(glm::vec3(0.f, 0.f, 0.f), m_SunDirection, glm::vec4{ -10.f, 10.f, -10.f, 10.f }, glm::vec2{ -10.f, 10.f });
		}

		void Update(Orion::Timestep deltaTime) override
		{

			Orion::CamerasController::Update(deltaTime);
			Orion::Renderer::LightSettings(m_LightSettings.x, m_LightSettings.y);

			ORI_PROFILE_FUNCTION();

			glm::vec3 lightPos{};
			float time = Orion::CurrentTime::GetCurrentTimeInSec();

			lightPos.x = sin(time) * 2.0f;
			lightPos.y = 6.0f;
			lightPos.z = cos(time) * 2.0f;

			m_PointLight->GetLightProperties().Position = lightPos / 3.f;
			m_PointLight->GetLightProperties().DiffuseLightColor =  m_Color;
			m_PointLight->GetLightProperties().SpecularLightColor = m_Color / 2.f;

			//m_PointLight2->GetLightProperties().Position = lightPos * 1.f;
			//m_PointLight2->GetLightProperties().DiffuseLightColor = glm::vec3(0.2f, m_Color.g, 0.2f);
			//m_PointLight2->GetLightProperties().SpecularLightColor = glm::vec3(0.2f, m_Color.g, 0.2f)  / 2.f;
			//
			//m_PointLight3->GetLightProperties().Position = lightPos * 4.f;
			//m_PointLight3->GetLightProperties().DiffuseLightColor = glm::vec3(0.2f, 0.2f, m_Color.b);
			//m_PointLight3->GetLightProperties().SpecularLightColor = glm::vec3(0.2f, m_Color.g, 0.2f)  / 2.f;


			m_SpotLight->GetLightProperties().Position = glm::vec3(cos(time) , 3.0f, sin(time)) ;
			m_DirLight->GetLightProperties().Direction = m_SunDirection;


			Orion::Renderer::BeginScene(Orion::CamerasController::GetActiveCamera(), m_FramebufferMS, [this]() {Render();});
			
				Render();
			
			Orion::Renderer::EndScene();

		
			m_FramebufferMS->BlitToBuffer(m_Framebuffer);


		}
		void Render() override 
		{
	
			
			Orion::Renderer::DrawModel(glm::translate(m_ModelMatrix, glm::vec3(0.0, 0.0, 1.0)), m_ModelCat);

			Orion::Renderer::DrawModel(glm::translate(glm::scale(m_ModelMatrix, glm::vec3(20.0f, 15.0f, 20.0f)), glm::vec3(-0.3, -0.3, 0.5)), m_ModelTree);


			auto matrix = glm::translate(m_ModelMatrix, glm::vec3(-0.1, -0.3, -3.2))  * glm::scale(m_ModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			


			Orion::Renderer::DrawModel(matrix, m_ModelCar);


			Orion::Renderer::DrawModel(glm::translate(glm::scale(m_ModelMatrix, glm::vec3(50.0f, 10.0f,50.0f)), glm::vec3(0.0, -0.4f, 0.0)), m_ModelPlatform);

			Orion::Material mat =
			{
				m_DiffuseMap, m_SpecularMap, 2.f
			};

			Orion::Renderer::DrawSphere(glm::mat4(1.0f),{nullptr,nullptr,0});
			Orion::Renderer::DrawCube(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-3.0f,0.f)), mat);

		}


		void OnEvent(Orion::Event& event) override
		{
			Orion::CamerasController::OnEvent(event);
			m_Dispatcher = Orion::EventDispatcher::CreateDispatcher(event);
			m_Dispatcher->Dispatch<Orion::KeyPressedEvent>(ORI_BIND_EVENT_FN(EditLayer3D::OnKeyPressed));
			m_Dispatcher->Dispatch<Orion::MouseMovedEvent>(ORI_BIND_EVENT_FN(EditLayer3D::OnMouseMoved));


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
			/*ImGui::Begin("Setting");
			ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
			ImGui::SliderFloat3("DirLight ", glm::value_ptr(m_SunDirection), -10.0f, 10.0f);
			ImGui::Text("FPS: %f", ts.GetFPS());

			bool show = true;*/
			//	ImGui::End();*/
				// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
		   // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
		   // In this specific demo, we are not using DockSpaceOverViewport() because:
		   // - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
		   // - we allow the host window to have padding (when opt_padding == true)
		   // - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
		   // TL;DR; this demo is more complicated than what you would normally use.
		   // If we removed all the options we are showcasing, this demo would become:
		   //     void ShowExampleAppDockSpace()
		   //     {
		   //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		   //     }
			static bool dockSpaceOpen = true;
			static bool opt_fullscreen = true;
			static bool opt_padding = false;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}
			else
			{
				dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
			// and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			if (!opt_padding)
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));


			ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
			if (!opt_padding)
				ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// Submit the DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}


			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Options"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
					ImGui::MenuItem("Padding", NULL, &opt_padding);
					ImGui::Separator();

					if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
					if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
					if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
					if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
					if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
					ImGui::Separator();

					if (ImGui::MenuItem("Close", NULL, false))
						dockSpaceOpen = false;
					ImGui::EndMenu();


				}

				ImGui::EndMenuBar();
			}
			ImGui::Begin("Setting");
			ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
			ImGui::SliderFloat3("DirLight ", glm::value_ptr(m_SunDirection), -1.0f, 1.0f);
			ImGui::SliderFloat("LinearAttenuation", &m_LightSettings.x, 0.0001f, 1.f);
			ImGui::SliderFloat("QuadraticAttenuation", &m_LightSettings.y, 0.0001f, 1.0f);
			ImGui::Text("FPS: %f", ts.GetFPS());


			ImGui::End();

			ImGui::Begin("ShadowMapDir");

			ImVec2& sizeDirView = ImGui::GetContentRegionAvail();
			
			ImGui::Image((void*)m_DirLight->GetShadowmap()->GetRendererID(), sizeDirView, ImVec2{0,1}, ImVec2{1,0});

			ImGui::End();


			ImGui::Begin("ShadowMapSpot");

			ImVec2& sizeSpitView = ImGui::GetContentRegionAvail();

			ImGui::Image((void*)m_SpotLight->GetShadowmap()->GetRendererID(), sizeSpitView, ImVec2{ 0,1 }, ImVec2{ 1,0 });

			ImGui::End();


			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0});

			ImGui::Begin("Viewport");

			bool isHover = ImGui::IsWindowHovered();
			bool isFocused = ImGui::IsWindowFocused();


			Application::Get().GetImGuiLayer()->SetBlockEvent(!isFocused || !isHover);
			CamerasController::SetBlockUpdate(!isHover);


			ImVec2& size = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *(glm::vec2*)&size)
			{
				m_ViewportSize = { size.x,size.y };
				m_FramebufferMS->Resize(m_ViewportSize.x, m_ViewportSize.y);
				m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

				CamerasController::OnViewportResize(m_ViewportSize);
			}
			m_ViewportSize = {size.x,size.y};
				
			ImGui::Image((void*)m_Framebuffer->GetColorAttachmentID(), size, ImVec2{0,1}, ImVec2{1,0});

			ImGui::End();

			ImGui::PopStyleVar();

			ImGui::End();

		


			/*auto& stats = Orion::Renderer::GetStats();


			ImGui::Text("DrawCalls: %d", stats.GetTotalDrawCalls());
			ImGui::Text("Quads: %d", stats.GetTotalQuadCount());
			ImGui::Text("QuadIndicies: %d", stats.GetTotalQuadIndexCount());
			ImGui::Text("Lines: %d", stats.GetTotalLineCount());
			ImGui::Text("Verticies: %d", stats.GetTotalVertexCount());*/





		}
	private:
		Orion::Shared<Orion::DummyCamera> m_Camera, m_LightDirCam;
		Orion::Shared<Orion::EventDispatcher> m_Dispatcher;
		glm::vec4 m_Color{ 0.842f, 0.523f, 0.768f, 1.0f };
		glm::vec3 m_Position{ 0,0,0 };
		glm::vec3 m_SunDirection{ 0.1f,-1.f,0.1f };

		glm::vec2 m_LightSettings{0.15f,0.058f};
		glm::vec2 m_ViewportSize;
		glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
		Orion::Shared<Orion::Framebuffer> m_FramebufferMS, m_Framebuffer, m_Framebuffer_Refra, m_ShadowMapDir, m_ShadowMapPoint, m_ShadowMapSpot;
		Orion::Shared<Orion::Model> m_ModelCat, m_ModelPlatform, m_ModelLamp, m_ModelTree, m_ModelCar;
		Orion::Shared<Orion::LightSource> m_SpotLight, m_DirLight, m_PointLight, m_PointLight2, m_PointLight3;
		Orion::Shared<Orion::Texture2D> m_DiffuseMap, m_SpecularMap, m_CubeMap, m_SkyTexture;
	};
}