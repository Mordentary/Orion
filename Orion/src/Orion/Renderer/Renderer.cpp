#include "oripch.h"
#include "Renderer.h"
#include"Platform/OpenGL/OpenGLShader.h"


namespace Orion
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<SceneData>();

	void Renderer::BeginScene(const Ref<OrthographicCamera>& m_Camera)
	{
		s_SceneData->ViewProjectionMatrix = m_Camera->GetProjectionViewMatrix();
	}
	void Renderer::EndScene()
	{

	}
	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix)
	{
		shader->Bind();

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ModelMatrix", modelMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}



}
