#pragma once

#include<string>
#include<unordered_map>
#include"Orion/Renderer/Shader.h"

typedef unsigned int GLenum;

namespace Orion
{
	class OpenGLShader : public Shader
	{
	public:
        OpenGLShader(const std::string& srcPath);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	


        uint32_t GetUniformLocation(const std::string& name) const;


        // Uiform-functions 
        void UploadUniformBool(const std::string& name, bool value);

        void UploadUniformInt(const std::string& name, int value);

        void UploadUniformFloat(const std::string& name, float value);

        void UploadUniformFloat2(const std::string& name, const glm::vec2& value);

        void UploadUniformFloat2(const std::string& name, float x, float y);

        void UploadUniformFloat3(const std::string& name, const glm::vec3& value);

        void UploadUniformFloat3(const std::string& name, float x, float y, float z);

        void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

        void UploadUniformFloat4(const std::string& name, float x, float y, float z, float w);

        void UploadUniformMat2(const std::string& name, const glm::mat2& mat);

        void UploadUniformMat3(const std::string& name, const glm::mat3& mat);

        void UploadUniformMat4(const std::string& name, const glm::mat4& mat);

    private:
        std::string ReadFile(const std::string& srcPath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
        mutable std::unordered_map<std::string, uint32_t> m_UniformLocationCache;
	};
}
