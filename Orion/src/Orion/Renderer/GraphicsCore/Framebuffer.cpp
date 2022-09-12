#include "oripch.h"
#include "Framebuffer.h"

#include "../GraphicsRendering/Renderer.h"
#include"Platform/OpenGL/OpenGLFramebuffer.h"

namespace Orion
{
	Scoped<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return  CreateScoped<OpenGLFramebuffer>(spec);
			break;
		}
	}

}