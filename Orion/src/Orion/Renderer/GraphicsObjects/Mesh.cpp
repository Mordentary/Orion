#include "oripch.h"
#include "Mesh.h"

namespace Orion 
{
	Mesh::Mesh(const Shared<Mesh>& mesh)
	{
		m_Material = mesh->GetMaterial();
		m_Indices.reserve(mesh->GetIndicesCount());
		m_Vertices.reserve(mesh->GetVerticesCount());


		memcpy(m_Vertices.data(), mesh->GetVerticesData(), sizeof(MeshVertex) * mesh->GetVerticesCount());
		memcpy(m_Indices.data(), mesh->GetVerticesData(), sizeof(uint32_t) * mesh->GetIndicesCount());



		SetupMesh();
	}


	void Mesh::Render() 
	{
		
		RenderCommand::DrawIndexed(m_MeshVertexArray, m_Indices.size());
	}

	void Mesh::SetupMesh()
	{

		m_MeshVertexArray = VertexArray::Create();
		Shared<IndexBuffer> indexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());
		m_MeshVertexArray->SetIndexBuffer(indexBuffer);


		Shared<VertexBuffer> vertexBuffer = VertexBuffer::Create((float*)&m_Vertices[0], (uint32_t)m_Vertices.size() * sizeof(MeshVertex));
		vertexBuffer->SetLayout({
		{Orion::ShaderDataType::Float3, "a_Position"},
		{Orion::ShaderDataType::Float3, "a_Normal"},
		{Orion::ShaderDataType::Float3, "a_Tangent"},
		{Orion::ShaderDataType::Float3, "a_Bitangent"},
		{Orion::ShaderDataType::Float4, "a_Color"},
		{Orion::ShaderDataType::Float2, "a_TextureCoord"},
		{Orion::ShaderDataType::Float, "a_TextureSlot"}
			});

		m_MeshVertexArray->AddVertexBuffer(vertexBuffer);

	}

}