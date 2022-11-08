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
		static Scoped<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc);













	
		virtual void SetInt(const std::string& name, int value) = 0;

		virtual void SetIntArray(const std::string& name, const int* values, uint32_t count) = 0;

		virtual void SetFloat(const std::string& name, float value) = 0;

		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;

		virtual void SetFloat2(const std::string& name, float x, float y) = 0;

		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;

		virtual void SetFloat3(const std::string& name, float x, float y, float z) = 0;

		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual void SetFloat4(const std::string& name, float x, float y, float z, float w) = 0;
 
		virtual void SetMat4(const std::string& name, const glm::mat4& mat) = 0;
		 


	};



	class ShaderLibrary 
	{
	public:
		static void Add(const Shared<Shader>& shader);
		static void Add(const std::string& name,const Shared<Shader>& shader);
		static Shared<Shader> Load(const std::string& filepath);
		static Shared<Shader> Load(const std::string& name, const std::string& filepath);

		static Shared<Shader> Get(const std::string& name);
		static bool Exists(const std::string& name);

	private:
		static std::unordered_map<std::string, Shared<Shader>> m_Shaders;
	};


}

