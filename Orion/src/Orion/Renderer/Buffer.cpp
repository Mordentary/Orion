#include "oripch.h"
#include "Buffer.h"
#include "Renderer.h"
#include"Platform/OpenGL/OpenGLBuffer.h"

namespace Orion
{


	
	std::unique_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI()) 
		{
		case RendererAPI::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
			break;
		case RendererAPI::OpenGL:
			return  std::make_unique<OpenGLVertexBuffer>(vertices, size);
			break;
		}
	}



	std::unique_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
			break;
		case RendererAPI::OpenGL:
			return std::make_unique<OpenGLIndexBuffer>(indices, size);
			break;
		}
	}
	

}