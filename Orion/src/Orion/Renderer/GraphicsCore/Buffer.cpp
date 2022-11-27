#include "oripch.h"
#include "Buffer.h"
#include "../GraphicsRendering/Renderer.h"
#include"Platform/OpenGL/OpenGLBuffer.h"

namespace Orion
{

	
	Shared<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
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

	Shared<VertexBuffer> VertexBuffer::Create(uint32_t size)
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



	Shared<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
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
	

	Shared<UniformBuffer> UniformBuffer::Create(const BufferLayout& layout, const std::string& name, uint32_t numOfLayouts)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return  CreateScoped<OpenGLUniformBuffer>(layout, name, numOfLayouts);
			break;
		}
	}

	Shared<UniformBuffer> UniformBuffer::Create(uint32_t size, const std::string& name)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return  CreateScoped<OpenGLUniformBuffer>(size, name);
			break;
		}
	}

}