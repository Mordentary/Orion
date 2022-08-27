#include "oripch.h"
#include "Buffer.h"
#include "../GraphicsRendering/Renderer.h"
#include"Platform/OpenGL/OpenGLBuffer.h"

namespace Orion
{

	
	Scoped<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI()) 
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return  CreateScoped<OpenGLVertexBuffer>(vertices, size);
			break;
		}
	}

	Scoped<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return  CreateScoped<OpenGLVertexBuffer>(size);
			break;
		}
	}



	Scoped<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateScoped<OpenGLIndexBuffer>(indices, count);
			break;
		}
	}
	

}