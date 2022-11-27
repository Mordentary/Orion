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



			m_ModelCat = Orion::CreateShared<Orion::Model>("assets/models/Cat/Cat.obj");
			m_ModelPlatform = Orion::CreateShared<Orion::Model>("assets/models/Platform/platform.fbx");
			m_ModelLamp = Orion::CreateShared<Orion::Model>("assets/models/Lamp/source/SM_Lamp_01a.FBX");
			m_ModelTree = Orion::CreateShared<Orion::Model>("assets/models/Tree/Tree.obj");
			m_ModelCar = Orion::CreateShared<Orion::Model>("assets/models/Car/source/hw6.obj");
			m_ModelDragon = Orion::CreateShared<Orion::Model>("assets/models/Dragon/source/model.dae");
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

			m_PointLight = Orion::CreateShared<Orion::PointLight>(nullptr, 1024, 1024);
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
			
			
			Orion::Renderer::AddSceneCubemap(Texture2D::CreateCubemap("assets/textures/CubemapSea"));
			Orion::Renderer::AddSceneCubemap(Texture2D::CreateCubemap("assets/textures/CubemapJapan"));


			Orion::FramebufferSpecification specFB;
			specFB.Width = Orion::Application::Get().GetWindow().GetWidth();
			specFB.Height = Orion::Application::Get().GetWindow().GetHeight();
			
			m_FinalFramebuffer = Orion::Framebuffer::Create(specFB);

			//Orion::Renderer::SetSceneCubemap(m_PointLight->GetShadowmap());
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
			m_PointLight->GetGeneralLightProp().DiffuseLightColor =  m_Color;
			m_PointLight->GetGeneralLightProp().SpecularLightColor = m_Color / 2.f;

			m_ModelCar->SetPosition(m_ModelCar->GetPosition());

			m_SpotLight->GetGeneralLightProp().Position = glm::vec3(cos(time) , 3.0f, sin(time));

			m_DirLight->GetGeneralLightProp().Direction = m_SunDirection;
			m_DirLight->GetGeneralLightProp().DiffuseLightColor = m_SunColor;
			m_DirLight->GetGeneralLightProp().SpecularLightColor = m_SunColor / 2.f;

			
			
			Orion::Renderer::BeginScene(Orion::CamerasController::GetActiveCamera(), m_FinalFramebuffer, [this]() {Render();});
			
				Render();

			Orion::Renderer::EndScene();

			Orion::Renderer::PostProcessing(m_PostProcessSpec);

		}
		void Render() override 
		{


			Orion::Renderer::DrawScene();

			//Orion::CamerasController::GetActiveCamera()->Raycast(Input::GetLocalWindowMouseX(), Input::GetLocalWindowMouseY()).DebugDraw();
			

		}


		void OnEvent(Orion::Event& event) override
		{
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



					if (ImGui::MenuItem("Close", NULL, false))
						dockSpaceOpen = false;
					ImGui::EndMenu();


				}

				ImGui::EndMenuBar();
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Load"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
					ImGui::MenuItem("Padding", NULL, &opt_padding);
					ImGui::Separator();



					if (ImGui::MenuItem("Close", NULL, false))
						dockSpaceOpen = false;
					ImGui::EndMenu();


				}

				ImGui::EndMenuBar();
			}
			ImGui::Begin("Setting");


			if (ImGui::CollapsingHeader(("LightSetting"))) 
			{

				ImGui::ColorEdit4("PointLight Color", glm::value_ptr(m_Color));
				ImGui::Separator();

				ImGui::SliderFloat3("Sunlight Dir", glm::value_ptr(m_SunDirection), -1.0f, 1.0f);
				ImGui::ColorEdit4("Sunlight Color", glm::value_ptr(m_SunColor));

				ImGui::Separator();

				ImGui::SliderFloat("Linear Attenuation", &m_LightSettings.x, 0.01f, 5.f);
				ImGui::SliderFloat("Quadratic Attenuation", &m_LightSettings.y, 0.01f, 5.0f);
			}
			if (ImGui::CollapsingHeader(("PostProcess")))
			{

				//ImGui::("PointLight Color", glm::value_ptr(m_Color));
				ImGui::SliderInt("Blur passes count", (int32_t*)& m_PostProcessSpec.NumberBlurPasses, 2, 20);
				ImGui::SliderFloat("Exposure", &m_PostProcessSpec.Exposure, 0.1f, 5.f);
				ImGui::Checkbox("Enable Bloom", (bool*)&m_PostProcessSpec.BloomEnable);

				ImGui::Checkbox("Enable HDR", (bool*)&m_PostProcessSpec.HDR_Enable);
				if (m_PostProcessSpec.HDR_Enable)
				{
				
					ImGui::Combo("Tone mapping model", &m_PostProcessSpec.HDR_CurrentModel, "ACES_Narkowicz\0Reinhard\0ReinhardExt\0ReinhardExtLuminence\0ReinhardJodie\0HableFilmic\0");
					//ORI_INFO("Index: {0}", m_PostProcessSpec.HDR_CurrentModel);

					if (m_PostProcessSpec.HDR_CurrentModel == 2 || m_PostProcessSpec.HDR_CurrentModel == 3)
					{
						ImGui::SliderFloat("Reinhard whit point", &m_PostProcessSpec.ReinhardWhitePoint, 0.001f, 10.0f);
					}

				}

				ImGui::Checkbox("Enable GammaCorrection", (bool*)&m_PostProcessSpec.GammaCorrectionEnable);
				if (m_PostProcessSpec.GammaCorrectionEnable) 
				{

					ImGui::SliderFloat("Gamma factor", &m_PostProcessSpec.GammaFactor, 0.1f, 3.0f);

				}

				ImGui::Separator();

				ImGui::Checkbox("Enable Cubemaps", &m_PostProcessSpec.EnableCubemap);
				if (m_PostProcessSpec.EnableCubemap) 
				{
					ImGui::SliderInt("Cubemap selector:", (int32_t*)&m_PostProcessSpec.CubemapIndex, 0, Orion::Renderer::GetSceneCubemapCount() - 1);
				}

			}

			if (ImGui::CollapsingHeader(("Performance")))
			{
				auto& stats = Orion::Renderer::GetStats();

				ImGui::Text("TotalMs: %f | FPS: %f", ts.GetSeconds(), ts.GetFPS());
				ImGui::Separator();

				ImGui::Text("Shadow Mapping: %f", stats.GetTotalTimeShadowMappingPass());
				ImGui::Separator();

				ImGui::Text("Post-Process: %f", stats.GetTotalTimePostProcessPass());
			}
			
			ImGui::Checkbox("Enable deferred pipeline", &Orion::Renderer::IsPipelineDeferred());



			ImGui::End();

			ImGui::Begin("ShadowMapDir");

			ImVec2& sizeDirView = ImGui::GetContentRegionAvail();
			
			ImGui::Image((void*)m_DirLight->GetShadowmap()->GetRendererID(), sizeDirView, ImVec2{0,1}, ImVec2{1,0});

			ImGui::End();


			ImGui::Begin("ShadowMapSpot");

			ImVec2& sizeSpotView = ImGui::GetContentRegionAvail();

			ImGui::Image((void*)m_SpotLight->GetShadowmap()->GetRendererID(), sizeSpotView, ImVec2{ 0,1 }, ImVec2{ 1,0 });

			ImGui::End();


			ImGui::Begin("SelectedObject");

			ImVec2& winSize = ImGui::GetContentRegionAvail();
			float AR = winSize.x / winSize.y;

			auto& selectedObject = Orion::Renderer::GetSelectedModel();
			
			if (selectedObject) 
			{
				ImGui::Text("ModelName: %s", selectedObject->GetModelName().c_str());
				uint32_t index = 0;
			
				for (auto& mesh : selectedObject->GetMeshes())
				{
					
					if (ImGui::CollapsingHeader(("Mesh: " + std::to_string(index)).c_str()))
					{

						auto& mat = mesh->GetMaterial();

						if (mat.diffuseMap) 
						{
							ImGui::Text("DiffuseTexture:");
							ImGui::Image((void*)mat.diffuseMap->GetRendererID(), { (winSize.x / 2.f), (winSize.y / 2.f) * AR }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
							ImGui::Separator();

						}
						if (mat.specularMap) 
						{
							ImGui::Text("SpecularTexture:");
							ImGui::Image((void*)mat.specularMap->GetRendererID(), { (winSize.x / 2.f) , (winSize.y / 2.f) * AR }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
							ImGui::Separator();

						}
						if (mat.normalMap)
						{
							ImGui::Text("NormalMap:");
							ImGui::Image((void*)mat.normalMap->GetRendererID(), { (winSize.x / 2.f) , (winSize.y / 2.f) * AR }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
							ImGui::Separator();

						}

						ImGui::SliderFloat("Shininess factor: ", &mesh->GetMaterial().shininess, 10.0f, 128.f);
					}

						++index;
				}

			}

			ImGui::End();



			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0});



			ImGui::Begin("Viewport");

			bool isHover = ImGui::IsWindowHovered();
			bool isFocused = ImGui::IsWindowFocused();


			Application::Get().GetImGuiLayer()->SetBlockEvent(!isFocused || !isHover);
			CamerasController::SetBlockUpdate(!isHover);

			ImVec2& size = ImGui::GetContentRegionAvail();

			auto winA = ImGui::GetWindowPos();
			auto mouseA = ImGui::GetMousePos();

			
			
			Application::Get().GetWindow().SetSubWindowProp
			({ "Viewport", (int32_t)size.x, (int32_t)size.y, {winA.x,winA.y}, {mouseA.x,mouseA.y} });
			
			m_FinalFramebuffer->Resize(size.x, size.y);


			CamerasController::OnViewportResize({ size.x, size.y });
			
				
			ImGui::Image((void*)m_FinalFramebuffer->GetColorAttachmentID(0), size, ImVec2{0,1}, ImVec2{1,0});

			ImGui::End();

			//bool openDemo = true;
			//ImGui::ShowDemoWindow(&openDemo);


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
		glm::vec4 m_Color{ 0.842f, 0.523f, 0.768f, 1.0f }, m_SunColor{0.5f};
		glm::vec3 m_Position{ 0,0,0 };
		glm::vec3 m_SunDirection{ 0.1f,-1.f,0.1f };

		Orion::Renderer::PostProcessSpec m_PostProcessSpec{};
		glm::vec2 m_LightSettings{2.0f,0.100f};
		glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
		Orion::Shared<Orion::Framebuffer>  m_FinalFramebuffer;
		Orion::Shared<Orion::Model> m_ModelCat, m_ModelPlatform, m_ModelLamp, m_ModelTree, m_ModelCar, m_ModelDragon, m_ModelScene;
		Orion::Shared<Orion::LightSource> m_SpotLight, m_DirLight, m_PointLight, m_PointLight2, m_PointLight3;
		Orion::Shared<Orion::Texture2D> m_DiffuseMap, m_SpecularMap, m_CubeMap;
	};
}