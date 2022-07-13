#include "oripch.h"
#include "Shader.h"
#include "Renderer.h"
#include"Platform/OpenGL/OpenGLShader.h"

namespace Orion 
{

	Scope<Shader> Shader::Create(const std::string& srcPath) 
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return std::make_unique<OpenGLShader>(srcPath);
			break;
		}
	}

	Scope<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return std::make_unique<OpenGLShader>(vertexSrc, fragmentSrc);
			break;
		}
	}

}