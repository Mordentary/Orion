#include "oripch.h"

#include "Renderer.h"
#include "Renderer2D.h"
#include"Platform/OpenGL/OpenGLShader.h"
#include"../GraphicsObjects/LightSource.h"
#include"../GraphicsObjects/Mesh.h"
#include"../GraphicsObjects/Model.h"

namespace Orion
{
	struct RendererData3D
	{
		static const uint32_t MaxPolygons = 20000;
		static const uint32_t MaxVertices = MaxPolygons * 3;
		static const uint32_t MaxTextureSlots = 32;

		uint32_t MeshCount = 0;
		uint32_t MeshVertexDataOffset = 0;
		uint32_t MeshIndexkoDataOffset = 0;
		uint32_t DrawCalls = 0;
		uint32_t TotalSizeOfBatch = 0;

		Shared<VertexArray> MeshVertexArray = nullptr;
		Shared<IndexBuffer> MeshIndexBuffer = nullptr;
		Shared<VertexBuffer> MeshVertexBuffer = nullptr;

		Shared<Texture2D> WhiteTexture = nullptr;
		Shared<Texture2D> ShadowMap = nullptr;

		Shared<Mesh>* MeshBufferBase = nullptr;
		Shared<Mesh>* MeshIterator = nullptr;

		std::array<Shared<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotsIndex = 1;

		Shared<Shader> PhongShader = nullptr;
		Shared<Shader> LightShader = nullptr;
		Shared<Shader> CubemapShader = nullptr;

		Shared<Shader> DepthCubemapShader = nullptr;
		Shared<Shader> DepthTextureShader = nullptr;

		Shared<Shader> CurrentShader = nullptr;

		std::function<void()> SceneRenderFunc = nullptr;

		Shared<Framebuffer> ScreenFramebuffer = nullptr;

		Shared <Model> Cube = nullptr;
		Shared <Model> Sphere = nullptr;

		Shared<Texture2D> SceneCubeMap = nullptr;

		Material DefaultMaterial;
		std::vector<Shared<LightSource>> LightSources;
		Shared<DummyCamera> SceneCamera = nullptr;
	};

	RendererData3D Renderer::s_RenData3D;

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::Init()
	{
		ORI_PROFILE_FUNCTION();
		RenderCommand::Init();
		Renderer2D::Init();

		s_RenData3D.MeshVertexArray = VertexArray::Create();
		s_RenData3D.MeshVertexBuffer = VertexBuffer::Create(s_RenData3D.MaxPolygons * sizeof(MeshVertex));

		s_RenData3D.MeshVertexBuffer->SetLayout({
			{Orion::ShaderDataType::Float3, "a_Position"},
			{Orion::ShaderDataType::Float3, "a_Normal"},
			{Orion::ShaderDataType::Float4, "a_Color"},
			{Orion::ShaderDataType::Float2, "a_TextureCoord"},
			{Orion::ShaderDataType::Float, "a_TextureSlot"}
			});

		s_RenData3D.MeshIterator = s_RenData3D.MeshBufferBase;
		s_RenData3D.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_RenData3D.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t  samples[s_RenData3D.MaxTextureSlots];
		for (uint32_t i = 0; i < s_RenData3D.MaxTextureSlots; i++)
			samples[i] = i;

		//Shader preparing

		s_RenData3D.PhongShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/PhongShader.glsl");
		s_RenData3D.LightShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/LightShader.glsl");
		s_RenData3D.CubemapShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/CubemapShader.glsl");
		s_RenData3D.DepthCubemapShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/CubemapDepthShader.glsl");
		s_RenData3D.DepthTextureShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/TextureDepthShader.glsl");



		s_RenData3D.PhongShader->Bind();
		s_RenData3D.PhongShader->SetIntArray("u_Texture", &samples[0], s_RenData3D.MaxTextureSlots);

		s_RenData3D.TextureSlots[0] = s_RenData3D.WhiteTexture;

		s_RenData3D.DefaultMaterial =
		{
			s_RenData3D.WhiteTexture , s_RenData3D.WhiteTexture , 32.f
		};

		s_RenData3D.Cube = Orion::CreateShared<Orion::Model>("../Orion/src/Assets/models/PrimitiveShapes/Cube.obj");
		s_RenData3D.Sphere = Orion::CreateShared<Orion::Model>("../Orion/src/Assets/models/PrimitiveShapes/Sphere.obj");

		//ResetBatch();
	}

	//void Renderer::Flush()
	//{
	//	if (s_RenData3D.MeshCount) {
	//		s_RenData3D.MeshIterator = s_RenData3D.MeshBufferBase;

	//		for (uint32_t i = 0; i < s_RenData3D.MeshCount; i++)
	//		{
	//			auto& Mesh = **s_RenData3D.MeshIterator;

	//			uint32_t dataSize = sizeof(MeshVertex) * Mesh.GetVerticesCount();
	//			s_RenData3D.MeshVertexBuffer->SetData(s_RenData3D.MeshBufferBase, s_RenData3D.MeshVertexDataOffset, dataSize);

	//			s_RenData3D.MeshVertexDataOffset += dataSize;
	//			s_RenData3D.MeshIterator++;

	//			//	s_RenData3D.Inde->SetData(s_RenData3D.MeshBufferBase, s_RenData3D.MeshVertexDataOffset, dataSize);
	//		}

	//		s_RenData3D.PhongShader->Bind();
	//		RenderCommand::DrawIndexed(s_RenData3D.MeshVertexArray);
	//		s_RenData3D.DrawCalls++;

	//		memset(s_RenData3D.MeshBufferBase, 0, s_RenData3D.MeshVertexDataOffset);
	//		s_RenData3D.MeshVertexBuffer->SetData(s_RenData3D.MeshBufferBase, s_RenData3D.MeshVertexDataOffset);
	//	}
	//}
	//void Renderer::ResetBatch()
	//{
	//	s_RenData3D.MeshIterator = s_RenData3D.MeshBufferBase;
	//	s_RenData3D.MeshCount = 0;

	//	s_RenData3D.MeshVertexDataOffset = 0;
	//	s_RenData3D.TextureSlotsIndex = 1;
	//}


	


	void Renderer::PrepareLights()
	{
		for (auto& light : s_RenData3D.LightSources)
		{
			light->SetupLight(s_RenData3D.CurrentShader, s_RenData3D.LightSources, s_RenData3D.SceneRenderFunc);
		}
	}

	void Renderer::BeginScene(const Shared<DummyCamera>& camera, const Shared<Framebuffer>& screenFB, std::function<void()> renderFunc)
	{
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
	void Renderer::EndScene()
	{

		if (s_RenData3D.SceneCubeMap && !false)
			LoadCubemap(s_RenData3D.SceneCamera);


		s_RenData3D.ScreenFramebuffer->Unbind();
	}
	void Renderer::AddLight(const Shared<LightSource>& light)
	{
		if (light->GetLightModel() == nullptr)
		{
			light->SetLightModel(s_RenData3D.Sphere);
		}
		s_RenData3D.LightSources.push_back(light);
		
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

		s_RenData3D.Cube->GetMeshData()[0]->SetMaterial({ nullptr, nullptr, 0.0f });
		s_RenData3D.Cube->Render(s_RenData3D.CubemapShader);

		RenderCommand::SetDepthMask(true);


	}
	/*void Renderer::AppendMesh(const Shared<Mesh>& mesh, const glm::mat4& modelMatrix) //TODO: BATCH RENDERING
	{
		s_RenData3D.MeshIterator = &std::const_pointer_cast<Mesh>(mesh);

		s_RenData3D.TotalSizeOfBatch += sizeof(MeshVertex) * mesh->GetVerticesCount();
		s_RenData3D.MeshIterator++;
		s_RenData3D.MeshCount++;
	}*/

	void Renderer::DrawCube(const glm::mat4& modelMatrix, const Material& material)
	{
		
		s_RenData3D.CurrentShader->Bind();

		s_RenData3D.CurrentShader->SetMat4("u_ModelMatrix", modelMatrix);
		s_RenData3D.Cube->GetMeshData()[0]->SetMaterial(material);
		s_RenData3D.Cube->BindAllTexture();

		s_RenData3D.Cube->Render(s_RenData3D.CurrentShader);
	}

	void Renderer::DrawSphere(const glm::mat4& modelMatrix, const Material& material)
	{

		s_RenData3D.CurrentShader->Bind();
		s_RenData3D.CurrentShader->SetMat4("u_ModelMatrix", modelMatrix);

		s_RenData3D.Sphere->GetMeshData()[0]->SetMaterial(s_RenData3D.DefaultMaterial);
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
	int32_t Renderer::GetTextureSlot(const Shared<Texture2D>& texture)
	{
		int32_t textureIndex = 0;
		for (int32_t i = 1; i < s_RenData3D.TextureSlotsIndex; i++)
		{
			if ((*s_RenData3D.TextureSlots[i].get()) == (*texture.get()))
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0)
		{
			textureIndex = s_RenData3D.TextureSlotsIndex;
			s_RenData3D.TextureSlots[textureIndex] = texture;
			s_RenData3D.TextureSlotsIndex++;
		}
		return textureIndex;
	}
}