#pragma once

#include"../GraphicsCore/Texture.h"
#include"../GraphicsCore/VertexArray.h"
#include"../GraphicsCore/Buffer.h"
#include "../GraphicsCore/Shader.h"
#include "../GraphicsRendering/Renderer.h"
#include "../GraphicsRendering/Renderer2D.h"


namespace Orion
{

	struct Material 
	{
		Shared<Texture2D> Albedo = nullptr;
		Shared<Texture2D> NormalMap = nullptr;
		Shared<Texture2D> Mettalic = nullptr;
		Shared<Texture2D> Roughness = nullptr;
		Shared<Texture2D> Emission = nullptr;
		Shared<Texture2D> AO = nullptr;

		//For non PBR_MATERIALS
		float Shininess = 0.0f;
	};


	struct MeshVertex
	{

		//MeshVertex() = default;
		/*MeshVertex(MeshVertex&& mesh) : Position(std::move(mesh.Position)), Normal(std::move(mesh.Normal)), Tangent(std::move(mesh.Tangent)), Bitangent
		(std::move(mesh.Bitangent)), Color(std::move(mesh.Color)), TextureCoord(std::move(mesh.TextureCoord))
		{
			ORI_INFO("MOVED");
		}

		MeshVertex(const MeshVertex& mesh) : Position(mesh.Position), Normal(mesh.Normal), Tangent(mesh.Tangent), Bitangent
		(mesh.Bitangent), Color(mesh.Color), TextureCoord(mesh.TextureCoord)
		{
			ORI_INFO("COPIED");
		}*/

		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
		glm::vec4 Color;
		glm::vec2 TextureCoord;
	};
	class Mesh
	{
	public:
		Mesh() = default;

		Mesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, const Material& material, glm::vec3& min, glm::vec3& max) : m_Vertices(std::move(vertices)), m_Indices(std::move(indices)), m_AABBMin(std::move(min)), m_AABBMax(std::move(max)), m_DefaultMaterial(material)
		{
			m_CurrentMaterial = m_DefaultMaterial;
			SetupMesh();
		}

		inline MeshVertex* GetVerticesData() { return m_Vertices.data(); }
		inline uint32_t* GetIndicesData() { return m_Indices.data(); }
		inline Shared<VertexArray>& GetVertexArray() { return m_MeshVertexArray; }
		inline size_t GetVerticesCount() { return m_Vertices.size(); }
		inline size_t GetIndicesCount() { return m_Indices.size(); }

		inline  Material& GetCurrentMaterial() { return m_CurrentMaterial; }
		inline const Material& GetDefaultMaterial() { return m_DefaultMaterial; }

		void SetCurrentMaterial(const Material& material) { m_CurrentMaterial = material; }



		inline std::pair<glm::vec3&, glm::vec3&> GetAABB() { return {m_AABBMin, m_AABBMax}; }

		void Render(Shared<Shader>& shader);
	protected:
		void SetupMesh();

		Shared<VertexArray>m_MeshVertexArray;

		std::vector<MeshVertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		glm::vec3 m_AABBMin;
		glm::vec3 m_AABBMax;

		Material m_CurrentMaterial;
		Material m_DefaultMaterial;




	};
}
