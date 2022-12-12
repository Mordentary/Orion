#include "oripch.h"
#include "Mesh.h"

namespace Orion 
{

	void Mesh::Render(Shared<Shader>& shader)
	{
		

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