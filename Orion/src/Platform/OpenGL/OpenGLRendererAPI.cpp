#include "oripch.h"
#include "OpenGLRendererAPI.h"

#include"glad/glad.h"


namespace Orion {
	void OpenGLRendererAPI::Init() const
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_DEPTH_TEST);
	}
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Shared<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ?  indexCount : vertexArray->GetIndexBuffer()->GetCount() ;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D,0);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const
	{
		glViewport(x, y, width, height);

	}

}