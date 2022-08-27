#pragma once


#include"glm/glm.hpp"

#include"../GraphicsObjects/VertexArray.h"

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
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Shared<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Shared<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
		virtual void SetLineWidth(float width) = 0;
		inline static API GetAPI() { return s_RendererAPI; }
	private:
		static API s_RendererAPI;
	};

}