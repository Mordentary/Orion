#include "oripch.h"
#include "Texture.h"

#include"../GraphicsRendering/Renderer.h"
#include"Platform/OpenGL/OpenGLTexture.h"

namespace Orion {


	Shared<Texture2D> Texture2D::Create(const std::string& path, const Texture2DParameters& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateShared<OpenGLTexture2D>(path, spec);

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

	Shared<Texture2D> Texture2D::Create(const Shared<Framebuffer>& fb, bool depthAttach)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateShared<OpenGLTexture2D>(fb, depthAttach);

		}
	}
	Shared<Texture2D> Texture2D::CreateCubemap(const std::vector<std::string>& paths)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateShared<OpenGLTexture2D>(paths);

		}
	}
}