#pragma once

#include"glm/glm.hpp"

namespace Orion {

	class Shader
	{
	public:
		virtual ~Shader() = default;


		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;


		static Scope<Shader> Create(const std::string& srcPath);
		static Scope<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);

	};
}

