#include "oripch.h"

#include "Renderer.h"
#include "Renderer2D.h"
#include "Orion/Core/Application.h"

#include"Platform/OpenGL/OpenGLShader.h"
#include"../GraphicsObjects/LightSource.h"
#include"../GraphicsObjects/Mesh.h"
#include"../GraphicsObjects/Model.h"
#include "Orion/Core/KeyCodes.h"
#include "Orion/Core/MouseButtonCodes.h"
#include "Orion/Core/Input.h"

namespace Orion
{
	struct RendererData3D
	{
		Shared<Texture2D> WhiteTexture = nullptr;

		Shared<Shader> PhongShader = nullptr;
		Shared<Shader> LightShader = nullptr;
		Shared<Shader> CubemapShader = nullptr;

		Shared<Shader> DepthCubemapShader = nullptr;
		Shared<Shader> DepthTextureShader = nullptr;

		Shared<Shader> CurrentShader = nullptr;
		Shared<Shader> SelectModelShader = nullptr;

		Shared<Model> SelectedModel = nullptr;


		std::function<void()> SceneRenderFunc = nullptr;
		Shared<Framebuffer> ScreenFramebuffer = nullptr;


		Shared <Model> Cube = nullptr;
		Shared <Model> Sphere = nullptr;

		Shared<Texture2D> SceneCubeMap = nullptr;

		Material DefaultMaterial;
		std::vector<Shared<LightSource>> LightSources;
		std::vector<Shared<Model>> Models;

		Shared<DummyCamera> SceneCamera = nullptr;
	};

	RendererData3D Renderer::s_RenData3D;

	void Renderer::Init()
	{
		ORI_PROFILE_FUNCTION();
		RenderCommand::Init();
		Renderer2D::Init();

		
		s_RenData3D.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_RenData3D.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));


	
		//Shader preparing

		s_RenData3D.PhongShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/PhongShader.glsl");
		s_RenData3D.LightShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/LightShader.glsl");
		s_RenData3D.CubemapShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/CubemapShader.glsl");
		s_RenData3D.DepthCubemapShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/CubemapDepthShader.glsl");
		s_RenData3D.DepthTextureShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/TextureDepthShader.glsl");
		s_RenData3D.SelectModelShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/SelectModelShader.glsl");


		s_RenData3D.PhongShader->Bind();

		s_RenData3D.DefaultMaterial =
		{
			s_RenData3D.WhiteTexture , s_RenData3D.WhiteTexture, s_RenData3D.WhiteTexture,  32.f
		};

		s_RenData3D.Cube = Orion::CreateShared<Orion::Model>("../Orion/src/Assets/models/PrimitiveShapes/Cube.obj");
		s_RenData3D.Sphere = Orion::CreateShared<Orion::Model>("../Orion/src/Assets/models/PrimitiveShapes/Sphere.obj");

		
	}

	void Renderer::ClosestObjectToRayHit()
	{
		
		for (auto& model : s_RenData3D.Models)
		{
			if (model->IsIntersect(s_RenData3D.SceneCamera->Raycast(Input::GetLocalWindowMouseX(), Input::GetLocalWindowMouseY())) && Orion::Input::IsKeyPressed(ORI_KEY_F))
			{
				if (!s_RenData3D.SelectedModel || !s_RenData3D.SelectedModel->IsIntersect(s_RenData3D.SceneCamera->Raycast(Input::GetLocalWindowMouseX(), Input::GetLocalWindowMouseY())))
					s_RenData3D.SelectedModel = model;

				if (glm::distance(s_RenData3D.SceneCamera->GetPosition(), model->GetLastHitedPoint()) <
					glm::distance(s_RenData3D.SceneCamera->GetPosition(), s_RenData3D.SelectedModel->GetLastHitedPoint()))
					s_RenData3D.SelectedModel = model;
			}

		}
		if (s_RenData3D.SelectedModel)
			ORI_INFO("Intersect:{0} | Point:{1}", s_RenData3D.SelectedModel->GetModelName(),0);


	}

	void Renderer::PrepareLights()
	{
		for (auto& light : s_RenData3D.LightSources)
		{
			light->SetupLight(s_RenData3D.CurrentShader, s_RenData3D.LightSources, s_RenData3D.SceneRenderFunc);
		}
	}

	void Renderer::BeginScene(const Shared<DummyCamera>& camera, const Shared<Framebuffer>& screenFB, std::function<void()> renderFunc)
	{
		
		Renderer2D::BeginScene(camera);

		s_RenData3D.SceneCamera = camera;
		s_RenData3D.SceneRenderFunc = renderFunc;
		s_RenData3D.ScreenFramebuffer = screenFB;

		PrepareLights();


		s_RenData3D.ScreenFramebuffer->Bind();
		Orion::RenderCommand::SetClearColor(glm::vec4(0.850f, 0.796f, 0.937f, 1.0f));
		Orion::RenderCommand::Clear(ORI_CLEAR_COLOR | ORI_CLEAR_DEPTH | ORI_CLEAR_STENCIL);

		s_RenData3D.CurrentShader = s_RenData3D.PhongShader;

		s_RenData3D.WhiteTexture->Bind(0);

		s_RenData3D.LightShader->Bind();
		s_RenData3D.LightShader->SetMat4("u_ViewProj", camera->GetProjectionViewMatrix());

		s_RenData3D.PhongShader->Bind();
		s_RenData3D.PhongShader->SetMat4("u_ViewProj", camera->GetProjectionViewMatrix());
		s_RenData3D.PhongShader->SetFloat3("u_CameraPos", camera->GetPosition());
		LoadAndRenderLights();




	}

	void Renderer::DrawScene()
	{
		s_RenData3D.CurrentShader->Bind();
		for  (Shared<Model>& model : s_RenData3D.Models)
		{
			if (s_RenData3D.SelectedModel && model == s_RenData3D.SelectedModel) continue;


			s_RenData3D.CurrentShader->SetMat4("u_ModelMatrix", model->GetModelMatrix());
			model->Render(s_RenData3D.CurrentShader);


		}

		if (s_RenData3D.SelectedModel) 
		{
			glm::mat4 modelTranslate(0.0f);
			if (Orion::Input::IsMouseButtonPressed(ORI_MOUSE_BUTTON_1))
			{

				auto[mouseX,mouseY] = Orion::Input::GetLocalWindowMousePosition();

				auto viewport = Orion::Application::Get().GetWindow().GetSubWindowProp();
				// 0.. Width 

				//Width .. Width
				mouseX = ((viewport.Width*2) * mouseX / viewport.Width) - viewport.Width;
				mouseY = ((viewport.Height * 2) * mouseY / viewport.Height) - viewport.Height;


				mouseX /=  viewport.Width;
				mouseY /= viewport.Height;


				mouseX /= s_RenData3D.SelectedModel->GetMaxModelDivider();
				mouseY /= s_RenData3D.SelectedModel->GetMaxModelDivider();

				ORI_INFO("Mouse:{0} || {1}", mouseX, mouseY);

				modelTranslate = glm::translate(glm::mat4(1.0f),
					glm::vec3(
						s_RenData3D.SelectedModel->GetPosition().x + mouseX,
						s_RenData3D.SelectedModel->GetPosition().y + mouseY,
						s_RenData3D.SelectedModel->GetPosition().z)) * glm::scale(glm::mat4(1.0f), s_RenData3D.SelectedModel->GetScale());

				 s_RenData3D.SelectedModel->SetModelMatrix(modelTranslate);

			}

			s_RenData3D.CurrentShader->Bind();
			s_RenData3D.CurrentShader->SetMat4("u_ModelMatrix", s_RenData3D.SelectedModel->GetModelMatrix() );
			s_RenData3D.SelectedModel->Render(s_RenData3D.CurrentShader);

		}

	}


	void Renderer::EndScene()
	{
		 ClosestObjectToRayHit();
		Renderer2D::EndScene();

		if (s_RenData3D.SceneCubeMap && !false)
			LoadCubemap(s_RenData3D.SceneCamera);


		s_RenData3D.ScreenFramebuffer->Unbind();
	}
	void Renderer::AddLightToScene(const Shared<LightSource>& light)
	{
		if (light->GetLightModel() == nullptr)
		{
			light->SetLightModel(s_RenData3D.Sphere);
		}
		s_RenData3D.LightSources.push_back(light);
		
	}

	void Renderer::AddModelToScene(const Shared<Model>& model)
	{
		s_RenData3D.Models.push_back(model);
	}

	void Renderer::LoadAndRenderLights()
	{
		if (!s_RenData3D.LightSources.empty())
		{
			s_RenData3D.PhongShader->Bind();
			for (auto& lightSrc : s_RenData3D.LightSources)
			{
				lightSrc->LoadToLightShader();
			}

			for (auto& lightSrc : s_RenData3D.LightSources)
			{
				lightSrc->RenderLightModel(s_RenData3D.LightShader);
			}
		}

	}
	void Renderer::LightSettings(float linearAttenuation, float quadraticAttenuation) 
	{
		if (!s_RenData3D.LightSources.empty())
		{
			for (auto&lightSrc : s_RenData3D.LightSources)
			{
				lightSrc->GetLightProperties().LinearAttenuation = linearAttenuation;
				lightSrc->GetLightProperties().QuadraticAttenuation = quadraticAttenuation;
			}
		}
	}

	void Renderer::LoadCubemap(const Shared<DummyCamera>& camera)
	{

		RenderCommand::SetDepthMask(false);


		s_RenData3D.CubemapShader->Bind();

		s_RenData3D.SceneCubeMap->Bind(1);
		s_RenData3D.CubemapShader->SetInt("u_Cubemap", s_RenData3D.SceneCubeMap->GetCurrentSlot());
		s_RenData3D.CubemapShader->SetMat4("u_Proj", camera->GetProjectionMatrix());
		s_RenData3D.CubemapShader->SetMat4("u_View",glm::mat4(glm::mat3(camera->GetViewMatrix())));

		s_RenData3D.Cube->GetMeshData()[0]->SetMaterial({ nullptr, nullptr, nullptr,  0.0f });
		s_RenData3D.Cube->Render(s_RenData3D.CubemapShader);

		RenderCommand::SetDepthMask(true);


	}

	void Renderer::DrawCube(const glm::mat4& modelMatrix, const Material& material)
	{
		
		s_RenData3D.CurrentShader->Bind();

		s_RenData3D.CurrentShader->SetMat4("u_ModelMatrix", modelMatrix);
		s_RenData3D.Cube->GetMeshData()[0]->SetMaterial(material);
		s_RenData3D.Cube->SetModelMatrix(modelMatrix);

		s_RenData3D.Cube->BindAllTexture();

		s_RenData3D.Cube->Render(s_RenData3D.CurrentShader);
	}

	void Renderer::DrawSphere(const glm::mat4& modelMatrix, const Material& material)
	{

		s_RenData3D.CurrentShader->Bind();
		s_RenData3D.CurrentShader->SetMat4("u_ModelMatrix", modelMatrix);

		s_RenData3D.Sphere->GetMeshData()[0]->SetMaterial(s_RenData3D.DefaultMaterial);
		s_RenData3D.Sphere->SetModelMatrix(modelMatrix);

		s_RenData3D.Sphere->BindAllTexture();

		s_RenData3D.Sphere->Render(s_RenData3D.CurrentShader);
	}
	void Renderer::DrawModel(const glm::mat4& modelMatrix, const Shared<Model>& model)
	{

		s_RenData3D.CurrentShader->Bind();
		model->BindAllTexture();
		s_RenData3D.CurrentShader->SetMat4("u_ModelMatrix", modelMatrix);
		model->Render(s_RenData3D.CurrentShader);
	}
	void Renderer::SetSceneCubemap(const Shared<Texture2D>& cubeMap)
	{
		s_RenData3D.SceneCubeMap = cubeMap;
	}

}