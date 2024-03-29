#include "oripch.h"
#include "Shader.h"
#include "..//GraphicsRendering/Renderer.h"
#include"Platform/OpenGL/OpenGLShader.h"

namespace Orion 
{

	Scoped<Shader> Shader::Create(const std::string& srcPath) 
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateScoped<OpenGLShader>(srcPath);
			break;
		}
	}

	Scoped<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateScoped<OpenGLShader>(name, vertexSrc, fragmentSrc);
			break;
		}
	}

	Scoped<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ORI_CORE_ASSERT(false, "RendererAPI: None is currently none supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateScoped<OpenGLShader>(name, vertexSrc, fragmentSrc, geometrySrc);
			break;
		}
	}

	std::unordered_map<std::string, Shared<Shader>> ShaderLibrary::m_Shaders;

	void ShaderLibrary::Add(const Shared<Shader>& shader)
	{
		auto& name = shader->GetName();
		ORI_CORE_ASSERT(!Exists(name), "Shaders already exists!");
		m_Shaders[name] = shader;


	}
	void ShaderLibrary::Add(const std::string& name, const Shared<Shader>& shader) 
	{
		ORI_CORE_ASSERT(!Exists(name), "Shaders already exists!");
		m_Shaders[name] = shader;
	}
	Shared<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		Shared<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Shared<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		Shared<Shader> shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
	Shared<Shader> ShaderLibrary::Get(const std::string& name)
	{

		ORI_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

}