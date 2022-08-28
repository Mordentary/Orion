#pragma once

#include"Orion/Renderer/GraphicsCore/Texture.h"
#include"Orion/Renderer/GraphicsCore/VertexArray.h"
#include"Orion/Renderer/GraphicsCore/Buffer.h"



namespace Orion 
{



	struct Material {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
	};

	
	struct MeshVertex 
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec4 Color;
		glm::vec2 TextureCoord;
		float TextureSlot;

	};
	class Mesh
	{

	public:

		Mesh(const std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, const Shared<Texture2D>& texture) : m_Vertices(vertices), m_Indices(indices), m_Texture(texture)
		{
			SetupMesh();

		}

		inline MeshVertex* GetVerticesData() { return m_Vertices.data(); }
		inline uint32_t* GetIndicesData() { return m_Indices.data(); }
		inline Shared<VertexArray>& GetVertexArray() { return m_MeshVertexArray; }
		inline uint32_t GetVerticesCount() { return m_Vertices.size(); }
		inline uint32_t GetIndicesCount() { return m_Indices.size(); }


	private:
		void SetupMesh() 
		{

			m_MeshVertexArray = VertexArray::Create();
			Shared<IndexBuffer> indexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());
			m_MeshVertexArray->SetIndexBuffer(indexBuffer);


			Shared<VertexBuffer> vertexBuffer = VertexBuffer::Create((float*)&m_Vertices[0], (uint32_t)m_Vertices.size() * sizeof(MeshVertex));
			vertexBuffer->SetLayout({
			{Orion::ShaderDataType::Float3, "a_Position"},
			{Orion::ShaderDataType::Float3, "a_Normal"},
			{Orion::ShaderDataType::Float4, "a_Color"},
			{Orion::ShaderDataType::Float2, "a_TextureCoord"},
			{Orion::ShaderDataType::Float, "a_TextureSlot"}
			});

			m_MeshVertexArray->AddVertexBuffer(vertexBuffer);

		}
		Shared<VertexArray>m_MeshVertexArray;
		std::vector<MeshVertex> m_Vertices;
		Shared<Orion::Texture2D> m_Texture;
		std::vector<uint32_t> m_Indices;
		Material m_Material;


	};
}
