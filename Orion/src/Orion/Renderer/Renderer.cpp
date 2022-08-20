#include "oripch.h"

#include "Renderer.h"
#include "Renderer2D.h"
#include"Platform/OpenGL/OpenGLShader.h"


namespace Orion
{
	Scoped<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<SceneData>();

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::Init()
	{
		ORI_PROFILE_FUNCTION();
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::BeginScene(const Shared<OrthographicCamera>& m_Camera)
	{
		s_SceneData->ViewProjectionMatrix = m_Camera->GetProjectionMatrix();
	}
	void Renderer::EndScene()
	{

	}
	void Renderer::Submit(const Shared<Shader>& shader, const Shared<VertexArray>& vertexArray, const glm::mat4& modelMatrix)
	{
		shader->Bind();

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ModelMatrix", modelMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}



}
