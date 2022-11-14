#include "oripch.h"
#include "OpenGLRendererAPI.h"

#include"glad/glad.h"


namespace Orion {
	void OpenGLRendererAPI::Init() const
	{
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_MULTISAMPLE);

		glEnable(GL_STENCIL_TEST);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//glStencilMask(0x00);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

	void OpenGLRendererAPI::CullBackFace(bool cull)//TODO: REMOVE 
	{
		if (cull) 
		glCullFace(GL_BACK);
		else
		glCullFace(GL_FRONT);
	} 

	void OpenGLRendererAPI::DoDepthTest(bool enabled)//TODO: REMOVE 
	{

		if(enabled)
		glEnable(GL_DEPTH_TEST);
		else
		glDisable(GL_DEPTH_TEST);


	}
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear(int32_t mode)
	{
		glClear(mode);
	}

	void OpenGLRendererAPI::DrawIndexed(const Shared<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLRendererAPI::StencilWrite(bool enabled) 
	{
		if(enabled)
		glStencilMask(0xFF);
		else
		glStencilMask(0x00);


	}
	void OpenGLRendererAPI::StencilMode(uint32_t mode, int32_t num, uint32_t bitmask)
	{
		glStencilFunc(mode, num, bitmask);
	}

	void OpenGLRendererAPI::DrawLines(const Shared<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRendererAPI::DrawArray(const Shared<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		glPointSize(2.f);
		vertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const
	{
		glViewport(x, y, width, height);

	}

	void OpenGLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}

	void OpenGLRendererAPI::SetDepthMask(bool mode)
	{
		glDepthMask(mode ? GL_TRUE : GL_FALSE);
	}

}