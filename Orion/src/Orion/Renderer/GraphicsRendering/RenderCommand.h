#pragma once

#include"RendererAPI.h"

namespace Orion
{
	class RenderCommand
	{
	public:

		inline static void Init() 
		{
			s_RendererAPI->Init();
		}
		inline static void SetLineWidth(float width)
		{
			s_RendererAPI->SetLineWidth(width);
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport( x,  y,  width,  height);
		}

		inline static void SetDepthMask(bool mode)
		{
			s_RendererAPI->SetDepthMask(mode);
		}


		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void StencilWrite(bool enabled) 
		{
			s_RendererAPI->StencilWrite(enabled);

		}

		inline static void StencilMode(uint32_t mode, int32_t num, uint32_t bitmask)
		{
			s_RendererAPI->StencilMode(mode, num, bitmask);
		}

		inline static void DoDepthTest(bool enabled)
		{
			s_RendererAPI->DoDepthTest(enabled);
		}

		inline static void Clear(int32_t mode) 
		{
			s_RendererAPI->Clear(mode);
		}

		inline static void DrawIndexed(const Shared<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void CullBackFace(bool cull) //TODO: REMOVE
		{
			s_RendererAPI->CullBackFace(cull);
		}
	
		inline static void DrawLines(const Shared<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		}
		inline static void DrawArray(const Shared<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			s_RendererAPI->DrawArray(vertexArray, vertexCount);
		}


	private:
		static Scoped<RendererAPI> s_RendererAPI;
	};
	
}