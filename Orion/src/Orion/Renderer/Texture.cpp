#include "oripch.h"
#include "Texture.h"

#include"Renderer.h"
#include"Platform/OpenGL/OpenGLTexture.h"

namespace Orion {


	Scoped<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_unique<OpenGLTexture2D>(path);

		}
	}


	Scoped<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_unique<OpenGLTexture2D>(width, height);

		}
	}
}