#include "oripch.h"
#include "VertexArray.h"

#include"Renderer.h"
#include"Platform/OpenGL/OpenGLVertexArray.h"

namespace Orion
{

	std::unique_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
	
		case RendererAPI::OpenGL:
			return std::make_unique<OpenGLVertexArray>();
	
		}
	}

}