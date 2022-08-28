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

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear() 
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Shared<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
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