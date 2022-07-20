#pragma once

#include"glm/glm.hpp"
#include<unordered_map>


namespace Orion {

	class Shader
	{
	public:
		virtual ~Shader() = default;


		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;


		virtual inline std::string GetName() const = 0;

		static Scoped<Shader> Create(const std::string& srcPath);
		static Scoped<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

	};



	class ShaderLibrary 
	{
	public:
		void Add(const Shared<Shader>& shader);
		void Add(const std::string& name,const Shared<Shader>& shader);
		Shared<Shader> Load(const std::string& filepath);
		Shared<Shader> Load(const std::string& name, const std::string& filepath);

		Shared<Shader> Get(const std::string& name);
		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, Shared<Shader>> m_Shaders;
	};
}

