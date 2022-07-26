#include "oripch.h"
#include "VertexArray.h"

#include"Renderer.h"
#include"Platform/OpenGL/OpenGLVertexArray.h"

namespace Orion
{

	Scoped<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
	
		case RendererAPI::API::OpenGL:
			return std::make_unique<OpenGLVertexArray>();
	
		}

		ORI_CORE_ASSERT(false, "Uknown render API");
		return nullptr;

	}

}