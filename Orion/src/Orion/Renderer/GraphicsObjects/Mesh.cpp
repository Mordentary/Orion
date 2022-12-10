#include "oripch.h"
#include "Mesh.h"

namespace Orion 
{

	void Mesh::Render(Shared<Shader>& shader)
	{
		 shader->Bind();
		 if (shader == Orion::ShaderLibrary::Get("PhongShader")) 
		 {
			 if (m_CurrentMaterial.Albedo)
			 {
				 m_CurrentMaterial.Albedo->Bind(2);
				 shader->SetInt("u_Material.diffuse", m_CurrentMaterial.Albedo->GetCurrentSlot());
			 }
			 else shader->SetInt("u_Material.diffuse", 0);

			 if (m_CurrentMaterial.Roughness)
			 {
				 m_CurrentMaterial.Roughness->Bind(3);
				 shader->SetInt("u_Material.specular", m_CurrentMaterial.Roughness->GetCurrentSlot());
			 }
			 else shader->SetInt("u_Material.specular", 0);

			 if (m_CurrentMaterial.NormalMap)
			 {
				 m_CurrentMaterial.NormalMap->Bind(4);
				 shader->SetInt("u_Material.normals", m_CurrentMaterial.NormalMap->GetCurrentSlot());
			 }
			 else shader->SetInt("u_Material.normals", 0);

			 if (m_CurrentMaterial.Shininess)
				 shader->SetFloat("u_Material.shininess", m_CurrentMaterial.Shininess);
			 else  shader->SetFloat("u_Material.shininess", 16.0f);
		 }

		 if (shader == Orion::ShaderLibrary::Get("PBRShader"))
		 {

			 if (m_CurrentMaterial.Albedo)
			 {
				 m_CurrentMaterial.Albedo->Bind(2);
				 shader->SetInt("u_Material.albedo", m_CurrentMaterial.Albedo->GetCurrentSlot());
			 }
			 else shader->SetInt("u_Material.albedo", 0);


			 if (m_CurrentMaterial.Roughness)
			 {
				 m_CurrentMaterial.Roughness->Bind(3);
				 shader->SetInt("u_Material.roughness", m_CurrentMaterial.Roughness->GetCurrentSlot());
			 }
			 else shader->SetInt("u_Material.roughness", 1);

			 if (m_CurrentMaterial.Mettalic)
			 {
				 m_CurrentMaterial.Mettalic->Bind(4);
				 shader->SetInt("u_Material.mettalic", m_CurrentMaterial.Mettalic->GetCurrentSlot());
			 }
			 else shader->SetInt("u_Material.mettalic", 1);


			 if (m_CurrentMaterial.NormalMap)
			 {
				 m_CurrentMaterial.NormalMap->Bind(5);
				 shader->SetInt("u_Material.normals", m_CurrentMaterial.NormalMap->GetCurrentSlot());
			 }
			 else shader->SetInt("u_Material.normals", 0);


			 if (m_CurrentMaterial.Emission)
			 {

				 m_CurrentMaterial.Emission->Bind(6);
				 shader->SetInt("u_Material.emission", m_CurrentMaterial.Emission->GetCurrentSlot());
			 }
			 else shader->SetInt("u_Material.emission", 1);



			 if (m_CurrentMaterial.AO)
			 {
				 m_CurrentMaterial.AO->Bind(7);
				 shader->SetInt("u_Material.ao", m_CurrentMaterial.AO->GetCurrentSlot());
			 }
			 else shader->SetInt("u_Material.ao", 1);

		 }



		// RenderCommand::DrawArray(m_MeshVertexArray, m_Vertices.size());

		RenderCommand::DrawIndexed(m_MeshVertexArray, m_Indices.size());
	}

	void Mesh::SetupMesh()
	{

		m_MeshVertexArray = VertexArray::Create();
		m_MeshVertexArray->Bind();
		Shared<IndexBuffer> indexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());
		m_MeshVertexArray->SetIndexBuffer(indexBuffer);


		Shared<VertexBuffer> vertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(MeshVertex));
		vertexBuffer->SetLayout({
		{Orion::ShaderDataType::Float3, "a_Position"},
		{Orion::ShaderDataType::Float3, "a_Normal"},
		{Orion::ShaderDataType::Float3, "a_Tangent"},
		{Orion::ShaderDataType::Float3, "a_Bitangent"},
		{Orion::ShaderDataType::Float4, "a_Color"},
		{Orion::ShaderDataType::Float2, "a_TextureCoord"},
			});

		m_MeshVertexArray->AddVertexBuffer(vertexBuffer);

	}

}