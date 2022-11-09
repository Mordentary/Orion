#pragma once

#include<string>
#include<unordered_map>
#include"Orion/Renderer/GraphicsCore/Shader.h"

typedef unsigned int GLenum;

namespace Orion
{
	class OpenGLShader : public Shader
	{
	public:
        OpenGLShader(const std::string& srcPath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometryShader);

		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	
        virtual inline std::string GetName() const { return m_Name; }

        int32_t GetUniformLocation(const std::string& name) const;



        virtual void SetInt(const std::string& name, int value) override;

        void SetIntArray(const std::string& name,const int* values, uint32_t count) override;

        virtual void SetFloat(const std::string& name, float value) override;

        virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;

        virtual void SetFloat2(const std::string& name, float x, float y) override;

        virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;

        virtual void SetFloat3(const std::string& name, float x, float y, float z) override;

        virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;

        virtual void SetFloat4(const std::string& name, float x, float y, float z, float w) override;

        virtual void SetMat4(const std::string& name, const glm::mat4& mat) override;

        virtual void SetMat4Array(const std::string& name, const glm::mat4* mat, uint32_t count) override;


        // Uiform-functions 
        void UploadUniformBool(const std::string& name, bool value);

        void UploadUniformIntArray(const std::string& name, const int* values, uint32_t count);

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

        void UploadUniformMat4Array(const std::string& name, const glm::mat4* mat, uint32_t count);



    private:
        std::string ReadFile(const std::string& srcPath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
        std::string m_Name, m_Path;
        mutable std::unordered_map<std::string, uint32_t> m_UniformLocationCache;
	};
}
