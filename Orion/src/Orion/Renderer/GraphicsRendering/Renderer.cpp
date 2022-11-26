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
#include <glm/gtc/type_ptr.hpp>

namespace Orion
{
	struct RendererData3D
	{
		Shared<Texture2D> WhiteTexture = nullptr;

		Shared<Shader> PhongShader = nullptr;
		Shared<Shader> LightShader = nullptr;
		Shared<Shader> CubemapShader = nullptr;
		Shared<Shader> GaussianBlurShader = nullptr;
		Shared<Shader> PostProcessingShader = nullptr;


		Shared<Shader> DepthCubemapShader = nullptr;
		Shared<Shader> DepthTextureShader = nullptr;

		Shared<Shader> CurrentShader = nullptr;
		Shared<Shader> SelectModelShader = nullptr;


		Orion::Renderer::PostProcessSpec ScenePostProcessSpec;

		std::function<void()> SceneRenderFunc = nullptr;

		Shared<Model> SelectedModel = nullptr;
		Shared <Model> Cube = nullptr;
		Shared <Model> Sphere = nullptr;


		Material DefaultMaterial;

		std::vector<Shared<LightSource>> LightSources;
		std::vector<Shared<Texture2D>> SceneCubemaps;
		std::vector<Shared<Model>> Models;

		Shared<DummyCamera> SceneCamera = nullptr;

		Orion::Renderer::Statistics3D Stats;

		Shared<UniformBuffer> MatricesUniformBuffer = nullptr;
		Shared<UniformBuffer> PostProcessUniformBuffer = nullptr;

		Shared<Framebuffer> HorizontalPassBlur = nullptr;
		Shared<Framebuffer> VerticalPassBlur = nullptr;

		Shared<Framebuffer> MS_Framebuffer = nullptr;
		Shared<Framebuffer> FinalFramebuffer = nullptr;
		
		//////////////////////////////////////////////////////////////////////// Deffered Rendering

		Shared<Shader> GBufferShader = nullptr;
		Shared<Shader> DeferredShader = nullptr;


		Shared<Framebuffer> GBuffer = nullptr;
		Shared<Framebuffer> DeferredShadingBuffer = nullptr;

		bool DeferredPipeline = false;

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
		s_RenData3D.GaussianBlurShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/GaussianBlurShader.glsl");
		s_RenData3D.PostProcessingShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/PostProcessingShader.glsl");



		s_RenData3D.GBufferShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/GBufferShader.glsl");
		s_RenData3D.DeferredShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/DeferredShader.glsl");



		s_RenData3D.MatricesUniformBuffer = UniformBuffer::Create
		(
			{
				{Orion::ShaderDataType::Mat4, "u_ViewProj"}
			}, 

			"u_MatricesBuffer"
		);
		s_RenData3D.MatricesUniformBuffer->Bind(0);

		s_RenData3D.PhongShader->LinkUniformBuffer(s_RenData3D.MatricesUniformBuffer);
		s_RenData3D.LightShader->LinkUniformBuffer(s_RenData3D.MatricesUniformBuffer);
		s_RenData3D.SelectModelShader->LinkUniformBuffer(s_RenData3D.MatricesUniformBuffer);
		s_RenData3D.GBufferShader->LinkUniformBuffer(s_RenData3D.MatricesUniformBuffer);

		Orion::FramebufferSpecification specFB;
		specFB.Width =  Orion::Application::Get().GetWindow().GetWidth();
		specFB.Height = Orion::Application::Get().GetWindow().GetHeight();


		specFB.Samples = 4;
		specFB.ColorAttachments = 2;
		s_RenData3D.MS_Framebuffer = Orion::Framebuffer::Create(specFB);


		specFB.Samples = 1;
		specFB.ColorAttachments = 3;
		s_RenData3D.GBuffer = Orion::Framebuffer::Create(specFB);


		specFB.ColorAttachments = 2;
		s_RenData3D.DeferredShadingBuffer = Orion::Framebuffer::Create(specFB);


		specFB.ColorAttachments = 1;
		s_RenData3D.HorizontalPassBlur = Orion::Framebuffer::Create(specFB);
		s_RenData3D.VerticalPassBlur =	Orion::Framebuffer::Create(specFB); 
		
		s_RenData3D.PostProcessUniformBuffer = UniformBuffer::Create
		(
			{
				{Orion::ShaderDataType::Int, "u_Bloom"},
				{Orion::ShaderDataType::Int, "u_GammaCorrection"},
				{Orion::ShaderDataType::Int, "u_HDR"},

				{Orion::ShaderDataType::Float, "u_WhitePoint"},
				{Orion::ShaderDataType::Float, "u_ExposureFactor"},
				{Orion::ShaderDataType::Float, "u_GammaFactor"},

				{Orion::ShaderDataType::Int, "u_CurrentToneMappingModel"}, 

				{Orion::ShaderDataType::Int, "u_GaussianBlurTex"}, 
				{Orion::ShaderDataType::Int, "u_SceneTex "}

			},

			"u_PostProcessSettings"
		);
		
		s_RenData3D.PostProcessUniformBuffer->Bind(1);
		s_RenData3D.PostProcessingShader->LinkUniformBuffer(s_RenData3D.PostProcessUniformBuffer);




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


	}

	void Renderer::PrepareLights()
	{
		for (auto& light : s_RenData3D.LightSources)
		{
			light->SetupLight(s_RenData3D.CurrentShader, s_RenData3D.LightSources, s_RenData3D.SceneRenderFunc);
		}
	}

	void Renderer::BeginScene(const Shared<DummyCamera>& camera, const Shared<Framebuffer>& finalFramebuffer, std::function<void()> renderFunc)
	{
		
		//Renderer2D::BeginScene(camera);

		s_RenData3D.SceneCamera = camera;
		s_RenData3D.SceneRenderFunc = renderFunc;
		s_RenData3D.FinalFramebuffer = finalFramebuffer;
		
		const auto& mainSpec = s_RenData3D.FinalFramebuffer->GetFramebufferSpec();
		
        s_RenData3D.MatricesUniformBuffer->SetDataUsingLayout(0, (void*)glm::value_ptr(camera->GetProjectionViewMatrix()));
		

		//Orion::RenderCommand::SetClearColor(glm::vec4(0.850f, 0.796f, 0.937f, 1.0f));

		s_RenData3D.Stats.m_ShadowMappingPass.Start();
		PrepareLights();
		s_RenData3D.Stats.m_ShadowMappingPass.End();


		ClosestObjectToRayHit();

		s_RenData3D.HorizontalPassBlur->Resize(mainSpec.Width, mainSpec.Height);
		s_RenData3D.VerticalPassBlur->Resize(mainSpec.Width, mainSpec.Height);
		s_RenData3D.WhiteTexture->Bind(0);
		Orion::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));


		if (s_RenData3D.DeferredPipeline) 
		{
			s_RenData3D.GBuffer->Resize(mainSpec.Width, mainSpec.Height);
			s_RenData3D.DeferredShadingBuffer->Resize(mainSpec.Width, mainSpec.Height);

			GeometryPass();
		}
		else 
		{

			s_RenData3D.MS_Framebuffer->Resize(mainSpec.Width, mainSpec.Height);

			s_RenData3D.PhongShader->Bind();
			s_RenData3D.PhongShader->SetFloat3("u_CameraPos", camera->GetPosition());


			s_RenData3D.MS_Framebuffer->Bind();
			Orion::RenderCommand::Clear(ORI_CLEAR_COLOR | ORI_CLEAR_DEPTH | ORI_CLEAR_STENCIL);

			s_RenData3D.CurrentShader = s_RenData3D.PhongShader;
			LoadAndRenderLights(s_RenData3D.PhongShader);;
		}
	}

	void Renderer::DrawScene()
	{
		s_RenData3D.CurrentShader->Bind();

		Orion::RenderCommand::StencilMode(ORI_GL_ALWAYS, 1, 0xFF);
		Orion::RenderCommand::StencilWrite(false);

		for  (Shared<Model>& model : s_RenData3D.Models)
		{
			if (s_RenData3D.SelectedModel && model == s_RenData3D.SelectedModel) continue;


			s_RenData3D.CurrentShader->SetMat4("u_ModelMatrix", model->GetModelMatrix());
			model->Render(s_RenData3D.CurrentShader);
		}

		if (s_RenData3D.SelectedModel)
		{

			if (Orion::Input::IsMouseButtonPressed(ORI_MOUSE_BUTTON_1))
			{
				s_RenData3D.SceneCamera->DragObjectAlongCameraPlane(s_RenData3D.SelectedModel);
			}

			Orion::RenderCommand::StencilWrite(true);
			Orion::RenderCommand::StencilMode(ORI_GL_ALWAYS, 1, 0xFF);

			s_RenData3D.CurrentShader->SetMat4("u_ModelMatrix", s_RenData3D.SelectedModel->GetModelMatrix());
			s_RenData3D.SelectedModel->Render(s_RenData3D.CurrentShader);

			
		}
	}


	void Renderer::RenderSelectedModelOutline() 
	{

		if (s_RenData3D.SelectedModel)
		{
			s_RenData3D.SelectModelShader->Bind();

			Orion::RenderCommand::StencilMode(ORI_GL_NOTEQUAL, 1, 0xFF);
			Orion::RenderCommand::StencilWrite(false);

			//Orion::RenderCommand::DoDepthTest(false);


			s_RenData3D.SelectModelShader->SetMat4("u_ModelMatrix", s_RenData3D.SelectedModel->GetModelMatrix());
			s_RenData3D.SelectedModel->Render(s_RenData3D.SelectModelShader);

			Orion::RenderCommand::StencilMode(ORI_GL_ALWAYS, 1, 0xFF);
			Orion::RenderCommand::StencilWrite(true);
			//Orion::RenderCommand::DoDepthTest(true);


		}

	}

	void Renderer::PostProcessing(const PostProcessSpec& spec)
	{
		auto& prop = Orion::Application::Get().GetWindow().GetSubWindowProp();
		if (prop.Width <= 0 || prop.Height <= 0) return;

		s_RenData3D.Stats.m_PostProcessStageTimer.Start();

		s_RenData3D.ScenePostProcessSpec = spec;

		Shared<Framebuffer> sceneBuffer = nullptr;

		if (s_RenData3D.DeferredPipeline) 
		{
			sceneBuffer = s_RenData3D.DeferredShadingBuffer;
			s_RenData3D.GBuffer->BlitStencilToBuffer(sceneBuffer);
		}
		else sceneBuffer = s_RenData3D.MS_Framebuffer;

		sceneBuffer->Bind();
		RenderSelectedModelOutline();
		sceneBuffer->Unbind();



		sceneBuffer->BlitColorToBuffer(s_RenData3D.VerticalPassBlur, 1, 0);
	

		s_RenData3D.GaussianBlurShader->Bind();
		s_RenData3D.GaussianBlurShader->SetFloatArray("u_Weights", &spec.GaussianCurve[0], 5);


		for (uint32_t i = 0; i < spec.NumberBlurPasses && spec.BloomEnable; i++)
		{

			RenderCommand::SetClearColor(glm::vec4(0.0f));
			s_RenData3D.GaussianBlurShader->Bind();
			s_RenData3D.HorizontalPassBlur->Bind();

			RenderCommand::Clear(ORI_CLEAR_COLOR | ORI_CLEAR_DEPTH | ORI_CLEAR_STENCIL);

			s_RenData3D.VerticalPassBlur->GetColorAttachmentTexture(0)->Bind(7);
			s_RenData3D.GaussianBlurShader->SetInt("u_SceneTex", s_RenData3D.VerticalPassBlur->GetColorAttachmentTexture(0)->GetCurrentSlot());
			s_RenData3D.GaussianBlurShader->SetInt("u_IsHorizontalBlur", true);

			Renderer2D::DrawBillboard(s_RenData3D.GaussianBlurShader, s_RenData3D.SceneCamera, glm::vec2(0, 0), glm::vec2(prop.Width, prop.Height));


			s_RenData3D.HorizontalPassBlur->Unbind();



			s_RenData3D.GaussianBlurShader->Bind();

			s_RenData3D.VerticalPassBlur->Bind();
		
			RenderCommand::Clear(ORI_CLEAR_COLOR | ORI_CLEAR_DEPTH | ORI_CLEAR_STENCIL);

			s_RenData3D.HorizontalPassBlur->GetColorAttachmentTexture(0)->Bind(7);
			s_RenData3D.GaussianBlurShader->SetInt("u_SceneTex", s_RenData3D.HorizontalPassBlur->GetColorAttachmentTexture(0)->GetCurrentSlot());
			s_RenData3D.GaussianBlurShader->SetInt("u_IsHorizontalBlur", false);

			Renderer2D::DrawBillboard(s_RenData3D.GaussianBlurShader, s_RenData3D.SceneCamera, glm::vec2(0, 0), glm::vec2(prop.Width, prop.Height));



			s_RenData3D.VerticalPassBlur->Unbind();
		}

		sceneBuffer->Bind();
		if (s_RenData3D.SceneCubemaps.size() > spec.CubemapIndex && spec.EnableCubemap)
			DrawCubemap();
		sceneBuffer->Unbind();




		sceneBuffer->BlitColorToBuffer(s_RenData3D.HorizontalPassBlur, 0, 0); // Use horizontal buffer because don't want to create new buffer

		//Orion::RenderCommand::FramebufferSRGBEnable(spec.GammaCorrectionEnable);


		s_RenData3D.FinalFramebuffer->Bind();
		Orion::RenderCommand::Clear(ORI_CLEAR_COLOR | ORI_CLEAR_DEPTH | ORI_CLEAR_STENCIL);

		s_RenData3D.PostProcessingShader->Bind();

		s_RenData3D.VerticalPassBlur->GetColorAttachmentTexture(0)->Bind(8);
		s_RenData3D.HorizontalPassBlur->GetColorAttachmentTexture(0)->Bind(9);


		s_RenData3D.PostProcessingShader->SetInt("u_GaussianBlurTex", s_RenData3D.VerticalPassBlur->GetColorAttachmentTexture(0)->GetCurrentSlot());
		s_RenData3D.PostProcessingShader->SetInt("u_SceneTex", s_RenData3D.HorizontalPassBlur->GetColorAttachmentTexture(0)->GetCurrentSlot());

		s_RenData3D.PostProcessUniformBuffer->SetData(&spec, s_RenData3D.PostProcessUniformBuffer->GetLayout().GetStride());
		
		Renderer2D::DrawBillboard(s_RenData3D.PostProcessingShader, s_RenData3D.SceneCamera, glm::vec2(0, 0), glm::vec2(prop.Width, prop.Height));


		s_RenData3D.PostProcessingShader->Unbind();
		
		s_RenData3D.FinalFramebuffer->Unbind();

		//Orion::RenderCommand::FramebufferSRGBEnable(false);
		
		s_RenData3D.Stats.m_PostProcessStageTimer.End();



	}
	void Renderer::EndScene()
	{
		if(s_RenData3D.DeferredPipeline)
		LightingPass();

		//Renderer2D::EndScene();

		

	}

	void Renderer::GeometryPass()
	{
		s_RenData3D.GBuffer->Bind();
		RenderCommand::Clear(ORI_CLEAR_COLOR | ORI_CLEAR_DEPTH | ORI_CLEAR_STENCIL);
		s_RenData3D.CurrentShader = s_RenData3D.GBufferShader;

	}

	void Renderer::LightingPass() 
	{

		auto& prop = Orion::Application::Get().GetWindow().GetSubWindowProp();
		if (prop.Width <= 0 || prop.Height <= 0) return;

		s_RenData3D.DeferredShadingBuffer->Bind();
		RenderCommand::Clear(ORI_CLEAR_COLOR | ORI_CLEAR_DEPTH | ORI_CLEAR_STENCIL);

		s_RenData3D.GBuffer->GetColorAttachmentTexture(0)->Bind(10);
		s_RenData3D.GBuffer->GetColorAttachmentTexture(1)->Bind(11);
		s_RenData3D.GBuffer->GetColorAttachmentTexture(2)->Bind(12);
		s_RenData3D.DeferredShadingBuffer->ActivateDrawingToColorTexture(0);

		s_RenData3D.DeferredShader->Bind();

		s_RenData3D.DeferredShader->SetFloat3("u_CameraPos", s_RenData3D.SceneCamera->GetPosition());

		s_RenData3D.DeferredShader->SetInt("u_gPosition", s_RenData3D.GBuffer->GetColorAttachmentTexture(0)->GetCurrentSlot());
		s_RenData3D.DeferredShader->SetInt("u_gNormal", s_RenData3D.GBuffer->GetColorAttachmentTexture(1)->GetCurrentSlot());
		s_RenData3D.DeferredShader->SetInt("u_gAlbedoSpec", s_RenData3D.GBuffer->GetColorAttachmentTexture(2)->GetCurrentSlot());


		Renderer2D::DrawBillboard(s_RenData3D.DeferredShader, s_RenData3D.SceneCamera, glm::vec2(0, 0), glm::vec2(prop.Width, prop.Height));

		s_RenData3D.GBuffer->BlitDepthToBuffer(s_RenData3D.DeferredShadingBuffer);

		s_RenData3D.DeferredShadingBuffer->Unbind();


		s_RenData3D.DeferredShadingBuffer->Bind();

		s_RenData3D.DeferredShadingBuffer->ActivateDrawingToAllTextures();

		s_RenData3D.DeferredShadingBuffer->Bind();

		LoadAndRenderLights(s_RenData3D.DeferredShader);

		s_RenData3D.DeferredShadingBuffer->Unbind();




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


	void Renderer::AddCubeToScene(const glm::mat4& modelMatrix, const Material& material)
	{
		Model model = *s_RenData3D.Cube.get();

		model.GetMeshData()[0]->SetMaterial(material);
		model.SetModelMatrix(modelMatrix);

		s_RenData3D.Models.push_back(CreateShared<Model>(model));


	}
	void Renderer::AddSphereToScene(const glm::mat4& modelMatrix, const Material& material) 
	{
		Model model = *s_RenData3D.Sphere.get();

		model.GetMeshData()[0]->SetMaterial(material);
		model.SetModelMatrix(modelMatrix);

		s_RenData3D.Models.push_back(CreateShared<Model>(model));
	}

	void Renderer::LoadAndRenderLights(const Shared<Shader>& shader)
	{
		if (!s_RenData3D.LightSources.empty())
		{
			shader->Bind();
			for (auto& lightSrc : s_RenData3D.LightSources)
			{
				lightSrc->LoadToLightShader(shader);
			}

			for (auto& lightSrc : s_RenData3D.LightSources)
			{
				lightSrc->RenderLightModel(s_RenData3D.LightShader);
			}
		}

	}

	void Renderer::LoadLightsToShader(const Shared<Shader>& shader)
	{
		if (!s_RenData3D.LightSources.empty())
		{
			shader->Bind();
			for (auto& lightSrc : s_RenData3D.LightSources)
			{
				lightSrc->LoadToLightShader(shader);
			}
		}
	}


	void Renderer::RenderLights() 
	{
		if (!s_RenData3D.LightSources.empty())
		{
			for (auto& lightSrc : s_RenData3D.LightSources)
			{
				lightSrc->RenderLightModel(s_RenData3D.LightShader);
			}
		}

	}


	void Renderer::UpdateLightSettings(float linearAttenuation, float quadraticAttenuation) 
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

	void Renderer::DrawCubemap()
	{
		
		RenderCommand::StencilWrite(false);
		RenderCommand::DepthWrite(false);


		s_RenData3D.CubemapShader->Bind();

		s_RenData3D.SceneCubemaps[s_RenData3D.ScenePostProcessSpec.CubemapIndex]->Bind(1);

		s_RenData3D.CubemapShader->SetInt("u_Cubemap", s_RenData3D.SceneCubemaps[s_RenData3D.ScenePostProcessSpec.CubemapIndex]->GetCurrentSlot());

		s_RenData3D.CubemapShader->SetMat4("u_ViewProj", s_RenData3D.SceneCamera->GetProjectionMatrix()  * glm::mat4(glm::mat3(s_RenData3D.SceneCamera->GetViewMatrix())));

		s_RenData3D.Cube->GetMeshData()[0]->SetMaterial({ nullptr, nullptr, nullptr,  0.0f });
		s_RenData3D.Cube->Render(s_RenData3D.CubemapShader);

		RenderCommand::DepthWrite(true);
		RenderCommand::StencilWrite(true);

	}

	void Renderer::DrawCube(const glm::mat4& modelMatrix, const Material& material)
	{
		
		s_RenData3D.CurrentShader->Bind();

		s_RenData3D.CurrentShader->SetMat4("u_ModelMatrix", modelMatrix);
		s_RenData3D.Cube->GetMeshData()[0]->SetMaterial(material);
		s_RenData3D.Cube->SetModelMatrix(modelMatrix);


		s_RenData3D.Cube->Render(s_RenData3D.CurrentShader);
	}

	void Renderer::DrawSphere(const glm::mat4& modelMatrix, const Material& material)
	{

		s_RenData3D.CurrentShader->Bind();
		s_RenData3D.CurrentShader->SetMat4("u_ModelMatrix", modelMatrix);

		s_RenData3D.Sphere->GetMeshData()[0]->SetMaterial(s_RenData3D.DefaultMaterial);
		s_RenData3D.Sphere->SetModelMatrix(modelMatrix);


		s_RenData3D.Sphere->Render(s_RenData3D.CurrentShader);
	}
	void Renderer::DrawModel(const glm::mat4& modelMatrix, const Shared<Model>& model)
	{
		s_RenData3D.CurrentShader->Bind();
		s_RenData3D.CurrentShader->SetMat4("u_ModelMatrix", modelMatrix);
		model->Render(s_RenData3D.CurrentShader);
	}
	void Renderer::AddSceneCubemap(const Shared<Texture2D>& cubeMap)
	{
		s_RenData3D.SceneCubemaps.push_back(cubeMap);
	}
	size_t Renderer::GetSceneCubemapCount() { return s_RenData3D.SceneCubemaps.size(); }
	const Shared<Model>& Renderer::GetSelectedModel() { return s_RenData3D.SelectedModel; }

	Orion::Renderer::Statistics3D& Renderer::GetStats()
	{
		return s_RenData3D.Stats;
	}

	bool& Renderer::IsPipelineDeferred()
	{
		return s_RenData3D.DeferredPipeline;
	}


}