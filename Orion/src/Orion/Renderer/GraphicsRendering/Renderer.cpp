#include "oripch.h"

#include "Renderer.h"
#include "Renderer2D.h"
#include "../Geometry/Mesh.h"
#include"Platform/OpenGL/OpenGLShader.h"


namespace Orion
{
	Renderer::SceneData Renderer::s_SceneData;

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::Init()
	{
		ORI_PROFILE_FUNCTION();
		RenderCommand::Init();
		Renderer2D::Init();


		s_SceneData.MeshVertexArray = VertexArray::Create();
		s_SceneData.MeshVertexBuffer = VertexBuffer::Create(s_SceneData.MaxPolygons * sizeof(MeshVertex));



		s_SceneData.MeshVertexBuffer->SetLayout({
			{Orion::ShaderDataType::Float3, "a_Position"},
			{Orion::ShaderDataType::Float3, "a_Normal"},
			{Orion::ShaderDataType::Float2, "a_TextureCoord"},
			{Orion::ShaderDataType::Float, "a_TextureSlot"},
			{Orion::ShaderDataType::Float4, "a_Color"}
			});


	//	s_SceneData.MeshIndexBuffer = IndexBuffer::Create(quadIndices, s_SceneData.MaxIndices_Quad);
	//	s_SceneData.MeshVertexArray->AddVertexBuffer(s_SceneData.QuadVertexBuffer);

	//	s_SceneData.MeshBufferBase = new Shared<Mesh>[s_SceneData.MaxMeshesPerDrawCall];
		s_SceneData.MeshIterator = s_SceneData.MeshBufferBase;
		s_SceneData.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_SceneData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t  samples[s_SceneData.MaxTextureSlots];
		for (uint32_t i = 0; i < s_SceneData.MaxTextureSlots; i++)
			samples[i] = i;
	

		//Shader preparing

		//s_SceneData.PhongShader = Shader::Create("../Orion/src/Platform/OpenGL/DefaultShaders/PhongShader.glsl");

		//s_SceneData.PhongShader->Bind();
		//s_SceneData.PhongShader->SetIntArray("u_Texture", &samples[0], s_SceneData.MaxTextureSlots);

		s_SceneData.TextureSlots[0] = s_SceneData.WhiteTexture;

		//ResetBatch();


	}

	void Renderer::Flush()
	{
		
		if (s_SceneData.MeshCount) {


			

			s_SceneData.MeshIterator = s_SceneData.MeshBufferBase;

			for (uint32_t i = 0; i < s_SceneData.MeshCount; i++)
			{
				auto& Mesh = **s_SceneData.MeshIterator;
				
				uint32_t dataSize = sizeof(MeshVertex) * Mesh.GetVerticesCount();
				s_SceneData.MeshVertexBuffer->SetData(s_SceneData.MeshBufferBase, s_SceneData.MeshVertexDataOffset, dataSize);


				s_SceneData.MeshVertexDataOffset += dataSize;
				s_SceneData.MeshIterator++;


			//	s_SceneData.Inde->SetData(s_SceneData.MeshBufferBase, s_SceneData.MeshVertexDataOffset, dataSize);

			}


			s_SceneData.PhongShader->Bind();
			RenderCommand::DrawIndexed(s_SceneData.MeshVertexArray);
			s_SceneData.DrawCalls++;


			memset(s_SceneData.MeshBufferBase, 0, s_SceneData.MeshVertexDataOffset);
			s_SceneData.MeshVertexBuffer->SetData(s_SceneData.MeshBufferBase, s_SceneData.MeshVertexDataOffset);
		}

	}
	void Renderer::ResetBatch()
	{
		s_SceneData.MeshIterator = s_SceneData.MeshBufferBase;
		s_SceneData.MeshCount = 0;

		s_SceneData.MeshVertexDataOffset = 0;
		s_SceneData.TextureSlotsIndex = 1;
	}


	void Renderer::BeginScene(const Shared<DummyCamera>& camera)
	{
		//s_SceneData->PhongShader->SetMat4("u_ProjectionMat", camera->GetProjectionMatrix());
	}
	void Renderer::EndScene()
	{

	}
	void Renderer::AppendMesh(const Shared<Mesh> mesh, const glm::mat4& modelMatrix)
	{
		s_SceneData.MeshIterator = &std::const_pointer_cast<Mesh>(mesh);


		s_SceneData.TotalSizeOfBatch += sizeof(MeshVertex) * mesh->GetVerticesCount();
		s_SceneData.MeshIterator++;
		s_SceneData.MeshCount++;
	}

	int32_t Renderer::GetTextureSlot(const Shared<Texture2D>& textures)
	{
		return int32_t();
	}



}
