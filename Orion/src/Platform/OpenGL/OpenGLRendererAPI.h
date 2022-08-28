#pragma once
#include"Orion/Renderer/GraphicsRendering/RendererAPI.h"

namespace Orion {

	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void Init() const override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Shared<VertexArray>& vertexArray, uint32_t indexCount) override;
		virtual void DrawArray(const Shared<VertexArray>& vertexArray, uint32_t vertexCount) override;
		virtual void DrawLines(const Shared<VertexArray>& vertexArray, uint32_t vertexCount) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const override;
		virtual void SetLineWidth(float width) override;

	};

}


