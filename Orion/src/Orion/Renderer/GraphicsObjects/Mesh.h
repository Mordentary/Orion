#pragma once

#include"../GraphicsCore/Texture.h"
#include"../GraphicsCore/VertexArray.h"
#include"../GraphicsCore/Buffer.h"
#include "../GraphicsCore/Shader.h"
#include "../GraphicsRendering/Renderer.h"


namespace Orion 
{



	struct Material {
		Shared<Texture2D> diffuseMap;
		Shared<Texture2D> specularMap;
		float shininess;
	};

	
	struct MeshVertex 
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
		glm::vec4 Color;
		glm::vec2 TextureCoord;
		float TextureSlot;

	};
	class Mesh
	{

	public:
		Mesh() = default;

		Mesh(const std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, const Material& material) : m_Vertices(vertices), m_Indices(indices), m_Material(material)
		{
			SetupMesh();
		}

		Mesh(const Shared<Mesh>& mesh);
		

		inline MeshVertex* GetVerticesData() { return m_Vertices.data(); }
		inline uint32_t* GetIndicesData() { return m_Indices.data(); }
		inline Shared<VertexArray>& GetVertexArray() { return m_MeshVertexArray; }
		inline uint32_t GetVerticesCount() { return m_Vertices.size(); }
		inline uint32_t GetIndicesCount() { return m_Indices.size(); }
		inline Material& GetMaterial() { return m_Material; }
		void SetMaterial(const Material& material) { m_Material = material; }

		void Render();
	protected:
		void SetupMesh();
		Shared<VertexArray>m_MeshVertexArray;
		std::vector<MeshVertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		Material m_Material;


	};
}
