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
	
			Orion::CamerasController::AddCamera("OrthoCamera", Orion::CreateShared<Orion::OrthographicCamera>(glm::vec3(0.0, 0.0f, 0.0f)));
			
			Orion::CamerasController::AddCamera("PerspectiveCamera1", Orion::CreateShared<Orion::PerspectiveCamera>(glm::vec3(0.0, 0.0f, 2.0f), glm::vec3(0.0f), 90.f, glm::vec2{ 0.1f,100.f }));
			Orion::CamerasController::AddCamera("PerspectiveCamera2", Orion::CreateShared<Orion::PerspectiveCamera>(glm::vec3(0.0, 0.0f, 2.0f), glm::vec3(0.0f), 90.f, glm::vec2{0.1f,100.f}));

			m_DiffuseMap = Orion::Texture2D::Create("assets/textures/container.png");
			m_SpecularMap = Orion::Texture2D::Create("assets/textures/container_specular.png");

			Timer modelCreation("ModelCreationTimer");

			modelCreation.Start();

			m_ModelBath = Orion::CreateShared<Orion::Model>("assets/models/scene/sponza/NewSponza_Main_glTF_002.gltf", SHADING_MODELS::PBR);
			m_ModelDragon = Orion::CreateShared<Orion::Model>("assets/models/Dragon/source/model.dae", SHADING_MODELS::PBR);
			m_ModelLamp = Orion::CreateShared<Orion::Model>("assets/models/Lamp/source/SM_Lamp_01a.FBX", SHADING_MODELS::PBR);
			//m_ModelPlatform = Orion::CreateShared<Orion::Model>("assets/models/Platform/platform.fbx", SHADING_MODELS::PBR);
			//m_ModelCat = Orion::CreateShared<Orion::Model>("assets/models/Cat/Cat.obj", SHADING_MODELS::PBR);
			//m_ModelPlatform = Orion::CreateShared<Orion::Model>("assets/models/Platform/platform.fbx");
		/*	m_ModelTree = Orion::CreateShared<Orion::Model>("assets/models/Tree/Tree.obj");*/
			//m_ModelCar = Orion::CreateShared<Orion::Model>("assets/models/Car/source/hw6.obj", SHADING_MODELS::PBR);
			//m_ModelScene = Orion::CreateShared<Orion::Model>("assets/models/Scene/sponza/NewSponza_Main_Yup_002.fbx", SHADING_MODELS::PBR);


			//m_ModelCrate = Orion::CreateShared<Orion::Model>("assets/models/WoodenCrate/Crate.obj");
			//m_ModelShield= Orion::CreateShared<Orion::Model>("assets/models/Armor/Shield/model.dae");
		

			modelCreation.Stop();

			ORI_INFO("MODEL_CREATION_TIME: {0}", modelCreation.GetTimeSeconds());


		//	m_ModelScene->SetScale(glm::vec3(50.f));

		/*	m_ModelCat->SetPosition(glm::vec3(0.0, 0.0, 1.0));


		

			m_ModelTree->SetScale(glm::vec3(20.0f, 15.0f, 20.0f));
			m_ModelTree->SetPosition(glm::vec3(-0.3, -4.0f, 10.5));
*/
			/*m_ModelCar->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
			m_ModelCar->SetPosition(glm::vec3(-0.7, -0.7, -5.2));


			m_ModelPlatform->SetScale(glm::vec3(50.0f, 10.0f, 50.0f));
			m_ModelPlatform->SetPosition(glm::vec3(0.0, -4.0f, 5.0));
*/

			m_ModelDragon->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
			m_ModelDragon->SetPosition(glm::vec3(-0.6, -0.6, -6.2));

		
		
			//m_PointLight2 = Orion::CreateShared<Orion::PointLight>();
			//m_PointLight3 = Orion::CreateShared<Orion::PointLight>();

			m_PointLight = Orion::CreateShared<Orion::PointLight>(nullptr, 1024, 1024);
			m_SpotLight = Orion::CreateShared<Orion::SpotLight>(m_ModelLamp, 2048, 2048);
			m_DirLight = Orion::CreateShared<Orion::DirectionalLight>();

			//_SpotLight->GetGeneralLightProp().Direction = glm::vec3(0.0f, -1.0f, 0.0f);

		

			//Orion::Renderer::AddSphereToScene(glm::mat4(1.0f), { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr, 32.f });
			//Orion::Renderer::AddCubeToScene(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.f)), { nullptr,nullptr,nullptr, 32.f  });
			


		/*		//Orion::Renderer::AddModelToScene(m_ModelCat);
			Orion::Renderer::AddModelToScene(m_ModelTree);   */
			//Orion::Renderer::AddModelToScene(m_ModelCar);
			Orion::Renderer::AddModelToScene(m_ModelDragon);
			Orion::Renderer::AddModelToScene(m_ModelBath);

			//Orion::Renderer::AddModelToScene(m_ModelScene);

			//Orion::Renderer::AddModelToScene(m_ModelPlatform);

			//Orion::Renderer::AddModelToScene(m_ModelCrate);
			////Orion::Renderer::AddModelToScene(m_ModelScene);

			Orion::Renderer::AddLightToScene(m_DirLight);
			
			Orion::Renderer::AddLightToScene(m_SpotLight);
			Orion::Renderer::AddLightToScene(m_PointLight);

			
		/*	for (int32_t i = 0; i < 5; i++)
			{
				for (int32_t j = 0; j < 5; j++)
				{
					Shared<LightSource> light = Orion::CreateShared<Orion::PointLight>(nullptr, 1024, 1024, glm::vec3(-i, rand() % 5, -j));
					float randF = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float randFF = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

					light->GetGeneralLightProp().DiffuseLightColor = glm::vec3(0.576f * randFF, randF * 0.42f * randFF,  0.45f * randF);
					light->GetGeneralLightProp().SpecularLightColor = glm::vec3(0.576f * randFF, randF * 0.42f * randFF, 0.45f * randF) / 2.f;

					Orion::Renderer::AddLightToScene(light);
				}
			}*/

			//for (int32_t i = 0; i < 5; i++)
			//{
			//	for (int32_t j = 1; j < 5; j++)
			//	{
			//		Shared<LightSource>  light = Orion::CreateShared<Orion::SpotLight>(m_ModelLamp, 1024, 1024, glm::vec3(i, rand() % 5, j));
			//		float randF = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			//		float randFF = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			//		light->GetGeneralLightProp().DiffuseLightColor = glm::vec3(0.576f * randFF, randF * 0.42f * randFF, 0.45f * randF);
			//		light->GetGeneralLightProp().SpecularLightColor = glm::vec3(0.576f * randFF, randF * 0.42f * randFF, 0.45f * randF) / 2.f;
			//		light->GetGeneralLightProp().Direction = glm::vec3(0.0f, -1.0f, 0.0f);

			//		Orion::Renderer::AddLightToScene(light);
			//	}
			//}

			
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

			m_SpotLight->GetGeneralLightProp().Position = glm::vec3(cos(time) , -1.0f, sin(time));

			m_DirLight->GetGeneralLightProp().Direction = m_SunDirection;
			m_DirLight->GetGeneralLightProp().DiffuseLightColor = m_SunColor;

			
			
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
					Orion::CamerasController::SetActiveCamera("PerspectiveCamera1");
				}
				if (e.GetKeyCode() == ORI_KEY_3)
				{
					Orion::CamerasController::SetActiveCamera("PerspectiveCamera2");
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

				ImGui::SliderFloat("Linear Attenuation", &m_LightSettings.x, 0.001f, 5.f);
				ImGui::SliderFloat("Quadratic Attenuation", &m_LightSettings.y, 0.001f, 5.0f);
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

			if (ImGui::CollapsingHeader(("Performance and statistic")))
			{
				auto& stats = Orion::Renderer::GetStats();

				ImGui::Text("TotalMs: %f | FPS: %f", ts.GetSeconds(), ts.GetFPS());
				ImGui::Separator();

				ImGui::Text("Forward Render: %f", stats.GetTotalTimeForwardRenderPass());
				ImGui::Separator();

				ImGui::Text("Shadow Mapping: %f", stats.GetTotalTimeShadowMappingPass());
				ImGui::Separator();

				ImGui::Text("Post-Process: %f", stats.GetTotalTimePostProcessPass());
				ImGui::Separator();

				//ImGui::Text("Draw calls: %d", stats.GetTotalDrawCalls());


			}

			ImGui::Checkbox("Enable deferred pipeline", &Orion::Renderer::IsPipelineDeferred());


			if (Orion::Renderer::IsPipelineDeferred())
			{

				ImGui::Combo("Output texture", &m_PostProcessSpec.DeferredOutputTexture, "FinalImage\0Position\0Normals\0Albedo\0Emission\0");
				//ORI_INFO("Index: {0}", m_PostProcessSpec.HDR_CurrentModel);
			}

			ImGui::Checkbox("Enable PBR", &Orion::Renderer::IsPBR());

			if (ImGui::CollapsingHeader(("Visual debugging options")))
			{
				auto& opt = Orion::Renderer::GetVisualDebuggingOptions();


				ImGui::Checkbox("Render model AABB", &opt.RenderModelsAABB);

				ImGui::Separator();

				ImGui::Checkbox("Render frustums of other cameras", &opt.RenderOtherCamerasFrustum);

			}
			



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
				if (ImGui::CollapsingHeader("Transformation"))
				{

					static bool checkBoxScale = false;
					ImGui::Checkbox("Scale per axis", &checkBoxScale);
					if (checkBoxScale)
					{
						glm::vec3 scale = selectedObject->GetScale();
						ImGui::SliderFloat3("Scale factor", glm::value_ptr(scale), 0.1f, 30.f);
						selectedObject->SetScale(scale);

					}
					else
					{
						float scale = selectedObject->GetScale()[0];
						ImGui::SliderFloat("Scale factor", &scale, 0.1f, 30.f);
						selectedObject->SetScale(glm::vec3(scale));
					}


					glm::vec3 pos = selectedObject->GetPosition();
					ImGui::SliderFloat3("Position", glm::value_ptr(pos), -10.f, 10.f);
					selectedObject->SetPosition(pos);


					glm::vec3 rotation = selectedObject->GetRotationAngles();
					ImGui::SliderFloat3("Rotation ", glm::value_ptr(rotation), -180.f, 180.f);
					selectedObject->SetRotationAngles(rotation);

				}
				uint32_t index = 0;
				if (ImGui::CollapsingHeader("Material Properties"))
				{

					if(selectedObject->IsPBRModel())
					{
						static bool useCustomValue = false;
						ImGui::Checkbox("Use custom value", &useCustomValue);
						ImGui::Separator();

						if (useCustomValue) 
						{
							static float  metallic  = selectedObject->GetCustomMaterialValues().Metallic;
							static float  roughness = selectedObject->GetCustomMaterialValues().Roughness;
							static glm::vec3 emission = selectedObject->GetCustomMaterialValues().Emission;

							ImGui::SliderFloat("Metallic ", &metallic, 0.0f, 1.f);
							ImGui::SliderFloat("Roughness ", &roughness, 0.05f, 0.95f);
							ImGui::SliderFloat3("Emmision color ", glm::value_ptr(emission), 0.00f, 1.0f);

							selectedObject->SetCustomMaterialValues(roughness, metallic, emission);
							selectedObject->ApplyCustomMaterialValues();

						}
						else
						{
							selectedObject->ApplyDefaultMaterialValues();

						}

						for (auto& mesh : selectedObject->GetMeshes())
						{


							if (ImGui::CollapsingHeader(("Mesh: " + std::to_string(index)).c_str()))
							{
								
								auto& mat = mesh->GetCurrentMaterial();

								if (mat.Albedo)
								{
									ImGui::Text("Albedo texture:");
									ImGui::Image((void*)mat.Albedo->GetRendererID(), { (winSize.x / 2.f), (winSize.y / 2.f) * AR }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
									ImGui::Separator();

								}
								if (mat.Roughness)
								{
									ImGui::Text("Roughness texture:");
									ImGui::Image((void*)mat.Roughness->GetRendererID(), { (winSize.x / 2.f) , (winSize.y / 2.f) * AR }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
									ImGui::Separator();

								}
							
								if (mat.Mettalic)
								{
									ImGui::Text("Metallic texture:");
									ImGui::Image((void*)mat.Mettalic->GetRendererID(), { (winSize.x / 2.f), (winSize.y / 2.f) * AR }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
									ImGui::Separator();

								}
								if (mat.NormalMap)
								{
									ImGui::Text("Normal map:");
									ImGui::Image((void*)mat.NormalMap->GetRendererID(), { (winSize.x / 2.f) , (winSize.y / 2.f) * AR }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
									ImGui::Separator();

								}
								if (mat.Emission)
								{
									ImGui::Text("Emission map:");
									ImGui::Image((void*)mat.Emission->GetRendererID(), { (winSize.x / 2.f) , (winSize.y / 2.f) * AR }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
									ImGui::Separator();

								}
								if (mat.AO)
								{
									ImGui::Text("Ambient occlusion map:");
									ImGui::Image((void*)mat.AO->GetRendererID(), { (winSize.x / 2.f) , (winSize.y / 2.f) * AR }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
									ImGui::Separator();

								}
							}
							++index;
						}
					}
					else 
					{
						for (auto& mesh : selectedObject->GetMeshes())
						{

							if (ImGui::CollapsingHeader(("Mesh: " + std::to_string(index)).c_str()))
							{
								auto& mat = mesh->GetCurrentMaterial();

								if (mat.Albedo)
								{
									ImGui::Text("Diffuse texture:");
									ImGui::Image((void*)mat.Albedo->GetRendererID(), { (winSize.x / 2.f), (winSize.y / 2.f) * AR }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
									ImGui::Separator();

								}
								if (mat.Roughness)
								{
									ImGui::Text("Specular texture:");
									ImGui::Image((void*)mat.Roughness->GetRendererID(), { (winSize.x / 2.f) , (winSize.y / 2.f) * AR }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
									ImGui::Separator();

								}
								if (mat.NormalMap)
								{
									ImGui::Text("Normal map:");
									ImGui::Image((void*)mat.NormalMap->GetRendererID(), { (winSize.x / 2.f) , (winSize.y / 2.f) * AR }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
									ImGui::Separator();

								}

								ImGui::SliderFloat("Shininess factor: ", &mesh->GetCurrentMaterial().Shininess, 16.0f, 128.f);
							}
							++index;
						}
					}
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
		Orion::Shared<Orion::EventDispatcher> m_Dispatcher;
		glm::vec4 m_Color{ 0.842f, 0.523f, 0.768f, 1.0f }, m_SunColor{0.5f};
		glm::vec3 m_Position{ 0,0,0 };
		glm::vec3 m_SunDirection{ 0.1f,-1.f,0.1f };

		Orion::Renderer::PostProcessSpec m_PostProcessSpec{};
		glm::vec2 m_LightSettings{2.0f,0.100f};
		glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
		Orion::Shared<Orion::Framebuffer>  m_FinalFramebuffer;
		Orion::Shared<Orion::Model> m_ModelCat, m_ModelPlatform, m_ModelLamp, m_ModelTree, m_ModelCar, m_ModelDragon, m_ModelScene, m_ModelCrate, m_ModelShield, m_ModelArmor, m_ModelBath;
		Orion::Shared<Orion::LightSource> m_SpotLight, m_DirLight, m_PointLight, m_PointLight2, m_PointLight3;
		Orion::Shared<Orion::Texture2D> m_DiffuseMap, m_SpecularMap, m_CubeMap;
	};
}