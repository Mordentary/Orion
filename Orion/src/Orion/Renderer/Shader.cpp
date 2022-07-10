#include "oripch.h"
#include "Shader.h"
#include "Renderer.h"
#include"Platform/OpenGL/OpenGLShader.h"

namespace Orion 
{
	std::unique_ptr<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
			break;
		case RendererAPI::OpenGL:
			return std::make_unique<OpenGLShader>(vertexSrc, fragmentSrc);
			break;
		}
	}

}