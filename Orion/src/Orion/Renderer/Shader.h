#pragma once

namespace Orion {
	class Shader
	{
	public:
		virtual ~Shader() {}


		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;


		static std::unique_ptr<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);

	};
}

