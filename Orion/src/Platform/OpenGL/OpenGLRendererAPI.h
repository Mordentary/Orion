#pragma once
#include"Orion/Renderer/GraphicsRendering/RendererAPI.h"

namespace Orion {

	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void Init() const override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear(int32_t mode) override;
		virtual void DrawIndexed(const Shared<VertexArray>& vertexArray, uint32_t indexCount) override;
		virtual void DrawArray(const Shared<VertexArray>& vertexArray, uint32_t vertexCount) override;
		virtual void DrawLines(const Shared<VertexArray>& vertexArray, uint32_t vertexCount) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const override;
		virtual void SetLineWidth(float width) override;
		virtual void DepthWrite(bool mode) override;

		virtual void FramebufferSRGBEnable(bool mode) override;

		virtual void CullBackFace(bool cull) override; //TODO: REMOVE

		virtual void DoDepthTest(bool enabled) override;
		virtual void StencilMode(uint32_t mode, int32_t num, uint32_t bitmask) override;

		virtual void StencilWrite(bool enabled) override;
	};

}


