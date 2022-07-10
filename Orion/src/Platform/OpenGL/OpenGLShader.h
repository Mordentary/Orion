#pragma once

#include<string>
#include"Orion/Renderer/Shader.h"

namespace Orion
{
	class OpenGLShader : public Shader
	{
	public:

		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		void inline Bind() const override;
		void inline Unbind() const override;
	

	private:
		uint32_t m_RendererID;
	};
}
