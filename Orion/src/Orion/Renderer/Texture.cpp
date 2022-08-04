#include "oripch.h"
#include "Texture.h"

#include"Renderer.h"
#include"Platform/OpenGL/OpenGLTexture.h"

namespace Orion {


	Shared<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateShared<OpenGLTexture2D>(path);

		}
	}


	Shared<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateShared<OpenGLTexture2D>(width, height);

		}
	}
}