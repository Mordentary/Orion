#include "oripch.h"

#include "Renderer.h"
#include "Renderer2D.h"
#include"Platform/OpenGL/OpenGLShader.h"


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

		Shared<VertexArray> MeshVertexArray = nullptr;;
		Shared<IndexBuffer> MeshIndexBuffer = nullptr;;
		Shared<VertexBuffer> MeshVertexBuffer = nullptr;;

		Shared<Texture2D> WhiteTexture = nullptr;;

		Shared<Mesh>* MeshBufferBase = nullptr;
		Shared<Mesh>* MeshIterator = nullptr;


		std::array<Shared<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotsIndex = 1;

		Shared<Shader> PhongShader;

		Shared<Mesh> Cube = nullptr;
		Material DefaultMaterial;

		std::vector<Shared<LightSource>> LightSources;


		


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
		//Renderer2D::Init();


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

		s_RenData3D.PhongShader = Shader::Create("../Orion/src/Platform/OpenGL/DefaultShaders/PhongShader.glsl");

		s_RenData3D.PhongShader->Bind();
		s_RenData3D.PhongShader->SetIntArray("u_Texture", &samples[0], s_RenData3D.MaxTextureSlots);

		s_RenData3D.TextureSlots[0] = s_RenData3D.WhiteTexture;

		float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
		};
		std::vector<MeshVertex> meshVertices(36);
		uint32_t index = 0;
		for (MeshVertex& vert : meshVertices)
		{
			vert.Position.x = vertices[index];
			vert.Position.y = vertices[index + 1];
			vert.Position.z = vertices[index + 2];

			index += 3;

			vert.Normal.x = vertices[index];
			vert.Normal.y = vertices[index + 1];
			vert.Normal.z = vertices[index + 2];

			index += 3;

			vert.Color.x = 0.5f;
			vert.Color.y = 0.5f;
			vert.Color.z = 0.5f;
			vert.Color.w = 1.0f;

			vert.TextureCoord.x = vertices[index];
			vert.TextureCoord.y = vertices[index + 1];

			index += 2;

			vert.TextureSlot = 0;

		}
		std::vector<uint32_t> indices(36);

		s_RenData3D.DefaultMaterial =
		{
			s_RenData3D.WhiteTexture , s_RenData3D.WhiteTexture , 64.f
		};
		s_RenData3D.Cube = CreateShared<Mesh>(meshVertices,indices, s_RenData3D.DefaultMaterial);


		ResetBatch();


	}

	void Renderer::Flush()
	{
		
		if (s_RenData3D.MeshCount) {


			s_RenData3D.MeshIterator = s_RenData3D.MeshBufferBase;

			for (uint32_t i = 0; i < s_RenData3D.MeshCount; i++)
			{
				auto& Mesh = **s_RenData3D.MeshIterator;
				
				uint32_t dataSize = sizeof(MeshVertex) * Mesh.GetVerticesCount();
				s_RenData3D.MeshVertexBuffer->SetData(s_RenData3D.MeshBufferBase, s_RenData3D.MeshVertexDataOffset, dataSize);


				s_RenData3D.MeshVertexDataOffset += dataSize;
				s_RenData3D.MeshIterator++;


			//	s_RenData3D.Inde->SetData(s_RenData3D.MeshBufferBase, s_RenData3D.MeshVertexDataOffset, dataSize);

			}


			s_RenData3D.PhongShader->Bind();
			RenderCommand::DrawIndexed(s_RenData3D.MeshVertexArray);
			s_RenData3D.DrawCalls++;


			memset(s_RenData3D.MeshBufferBase, 0, s_RenData3D.MeshVertexDataOffset);
			s_RenData3D.MeshVertexBuffer->SetData(s_RenData3D.MeshBufferBase, s_RenData3D.MeshVertexDataOffset);
		}

	}
	void Renderer::ResetBatch()
	{
		s_RenData3D.MeshIterator = s_RenData3D.MeshBufferBase;
		s_RenData3D.MeshCount = 0;

		s_RenData3D.MeshVertexDataOffset = 0;
		s_RenData3D.TextureSlotsIndex = 1;
	}


	void Renderer::BeginScene(const Shared<DummyCamera>& camera, glm::vec3 sunPos)
	{
		s_RenData3D.PhongShader->Bind();
		s_RenData3D.PhongShader->SetMat4("u_ViewProj", camera->GetProjectionViewMatrix());
		s_RenData3D.PhongShader->SetFloat3("u_CameraPos", camera->GetPosition());

		LoadAllLights();

		static glm::vec3 lightPos;
		float time = Orion::CurrentTime::GetCurrentTimeInSec();
		lightPos.x = sin(time) * 1.0f ;
		lightPos.y = 1.f;
		lightPos.z = cos(time) * 1.0f;
		
		glm::mat4 lightMatrix = glm::translate(glm::mat4(1.0f),lightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


		DrawCube(lightMatrix, s_RenData3D.DefaultMaterial);


	}
	void Renderer::EndScene()
	{
		auto& light = LightSource::GetCountOfSpotLights();
		light = 0;
	}
	void Renderer::AddLight(const Shared<LightSource> light) 
	{
		s_RenData3D.LightSources.push_back(light);
	}
	void Renderer::LoadAllLights()
	{
		if (!s_RenData3D.LightSources.empty()) 
		{
			for (auto& lightSrc : s_RenData3D.LightSources)
			{
				lightSrc->LoadToShader(s_RenData3D.PhongShader);
			}
			s_RenData3D.PhongShader->SetFloat("u_PointLightCount", LightSource::GetCountOfPointLights());
			s_RenData3D.PhongShader->SetFloat("u_SpotLightCount", LightSource::GetCountOfSpotLights());
		}

	}

	void Renderer::AppendMesh(const Shared<Mesh> mesh, const glm::mat4& modelMatrix)
	{
		s_RenData3D.MeshIterator = &std::const_pointer_cast<Mesh>(mesh);


		s_RenData3D.TotalSizeOfBatch += sizeof(MeshVertex) * mesh->GetVerticesCount();
		s_RenData3D.MeshIterator++;
		s_RenData3D.MeshCount++;
	}


	void Renderer::DrawCube(glm::mat4 modelMatrix, const Material& material)
	{
		s_RenData3D.Cube->SetMaterial(material);
	
		s_RenData3D.Cube->GetMaterial().diffuseMap->Bind(s_RenData3D.TextureSlotsIndex);
		s_RenData3D.TextureSlotsIndex++;
		s_RenData3D.Cube->GetMaterial().specularMap->Bind(s_RenData3D.TextureSlotsIndex);

		s_RenData3D.TextureSlotsIndex = 1;

		s_RenData3D.PhongShader->SetMat4("u_ModelMatrix", modelMatrix);

		s_RenData3D.PhongShader->SetInt("u_Material.diffuse", s_RenData3D.Cube->GetMaterial().diffuseMap->GetCurrentSlot());
		s_RenData3D.PhongShader->SetInt("u_Material.specular", s_RenData3D.Cube->GetMaterial().specularMap->GetCurrentSlot());
		s_RenData3D.PhongShader->SetFloat("u_Material.shininess", s_RenData3D.Cube->GetMaterial().shininess);

		RenderCommand::DrawArray(s_RenData3D.Cube->GetVertexArray(), s_RenData3D.Cube->GetVerticesCount());
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
