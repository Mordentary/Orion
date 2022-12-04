#pragma once


#include"glm/glm.hpp"

#include"../GraphicsCore/VertexArray.h"

namespace Orion
{
	class RendererAPI
	{
	public:

		enum class API
		{
			None = 0, OpenGL = 1
		};

	public:
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const = 0;
		virtual void Init() const  = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear(int32_t mode) = 0;
		virtual void DrawIndexed(const Shared<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Shared<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
		virtual void DrawArray(const Shared<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
		virtual void SetLineWidth(float width) = 0;
		virtual void CullBackFace(bool cull) =0; //TODO: REMOVE
		virtual void DepthWrite(bool mode) = 0;

		virtual void FramebufferSRGBEnable(bool mode) = 0;

		virtual void DoFaceCulling(bool enable) = 0;

		virtual void DoDepthTest(bool enabled) = 0;
		virtual void StencilMode(uint32_t mode, int32_t num, uint32_t bitmask) = 0;
		virtual void StencilWrite(bool enabled) = 0;



		inline static API GetAPI() { return s_RendererAPI; }
	private:
		static API s_RendererAPI;
	};

}