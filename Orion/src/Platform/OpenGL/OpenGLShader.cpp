#include"oripch.h"
#include"OpenGLShader.h"

#include<fstream>
#include<array>
#include<glad/glad.h>
#include"glm/gtc/type_ptr.hpp"

namespace Orion 
{

	static GLenum ShaderTypeFromString(std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "geometry")
			return GL_GEOMETRY_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		ORI_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& srcPath)
	{
		
		std::string shaderSource = ReadFile(srcPath);
		auto shaderSources = PreProcess(shaderSource);
		Compile(shaderSources);

		// assets/shaders/Texture.glsl
		auto lastSlash = srcPath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = srcPath.rfind('.');
		auto count = lastDot == std::string::npos ? srcPath.size() - lastSlash : lastDot - lastSlash;
		m_Name = srcPath.substr(lastSlash, count);

	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc): m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);

	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometryShader) : m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		sources[GL_GEOMETRY_SHADER] = geometryShader;

		Compile(sources);

	}


	OpenGLShader::~OpenGLShader()
	{
		glDeleteShader(m_RendererID);
	}
	void OpenGLShader::Bind() const
	{
		ORI_PROFILE_FUNCTION();
		glUseProgram(m_RendererID);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}
	std::string OpenGLShader::ReadFile(const std::string& srcPath)
	{
		m_Path = srcPath;
		std::string result;
		std::ifstream in(srcPath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			ORI_CORE_ERROR("Could not open file {0}", srcPath);
		}
		return result;
	}
	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLuint, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			ORI_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			ORI_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified!");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, 
				pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

		}

		return shaderSources;
	}
	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		ORI_PROFILE_FUNCTION();
		GLuint program = glCreateProgram();
		ORI_CORE_ASSERT(3 >= shaderSources.size(), "Orion supports only 3 types of shaders!");
		std::array<GLuint, 3> glShaderIDs{};
		int glShaderIDIndex = 0;

		auto iterator = glShaderIDs.begin();
 		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;


			GLuint shader = glCreateShader(type);

			const GLchar* sourceCstr = source.c_str();
			glShaderSource(shader, 1, &sourceCstr, 0);


			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			
				glDeleteShader(shader);


				ORI_CORE_ERROR("{0}", infoLog.data());
				ORI_CORE_ASSERT(false, "Shader compilation failure!! Path: " + m_Path);
				break;
			}
			glAttachShader(program, shader);

			glShaderIDs[glShaderIDIndex++] = shader;
		
		}


		m_RendererID = program;



		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			
			for (auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			ORI_CORE_ERROR("{0}", infoLog.data());
			ORI_CORE_ASSERT(false, "Shader program link failure!!");
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
		}
	
	}


	int32_t OpenGLShader::GetUniformLocation(const std::string& name) const
	{
		ORI_PROFILE_FUNCTION();
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		{
			return m_UniformLocationCache[name];
		}

		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());

		if (location == -1) return -1;

		m_UniformLocationCache[name] = location;
		return m_UniformLocationCache[name];

	}


	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		UploadUniformInt(name, value);
	}
	void OpenGLShader::SetIntArray(const std::string& name, const int* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}
	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		UploadUniformFloat(name, value);
	}
	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		UploadUniformFloat2(name, value);

	}
	void OpenGLShader::SetFloat2(const std::string& name, float x, float y)
	{
		UploadUniformFloat2(name, x, y);

	}
	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformFloat3(name, value);
	}
	void OpenGLShader::SetFloat3(const std::string& name, float x, float y, float z)
	{
		UploadUniformFloat3(name, x, y, z);
	}
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}
	void OpenGLShader::SetFloat4(const std::string& name, float x, float y, float z, float w)
	{
		UploadUniformFloat4(name, x, y, z,w);
	}
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& mat)
	{
		UploadUniformMat4(name, mat);
	}
	void OpenGLShader::SetMat4Array(const std::string& name, const glm::mat4* mat, uint32_t count)
	{
		UploadUniformMat4Array(name, mat, count);
	}

	void OpenGLShader::UploadUniformBool(const std::string& uniformName, bool value)
	{
		GLint location = GetUniformLocation(uniformName);
		if (location == -1) return;
		glUniform1i(location, (int)value);
	}
	void OpenGLShader::UploadUniformIntArray(const std::string& uniformName, const int* values, uint32_t count)
	{
		GLint location = GetUniformLocation(uniformName);
		if (location == -1) return;
		glUniform1iv(location, count, values);
	}
	void OpenGLShader::UploadUniformInt(const std::string& uniformName, int value)
	{
		GLint location = GetUniformLocation(uniformName);
		if (location == -1) return;
		glUniform1i(location, value);
	}
	void OpenGLShader::UploadUniformFloat(const std::string& uniformName, float value)
	{
		GLint location = GetUniformLocation(uniformName);
		if (location == -1) return;
		glUniform1f(location, value);
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& uniformName, const glm::vec2& value)
	{
		GLint location = GetUniformLocation(uniformName);
		if (location == -1) return;
		glUniform2f(location, value.x, value.y);
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& uniformName, float x, float y)
	{
		GLint location = GetUniformLocation(uniformName);
		if (location == -1) return;
		glUniform2f(location, x, y);
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& uniformName, const glm::vec3& value)
	{
		GLint location = GetUniformLocation(uniformName);
		if (location == -1) return;
		glUniform3f(location, value.x, value.y, value.z);
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& uniformName, float x, float y, float z)
	{
		GLint location = GetUniformLocation(uniformName);
		if (location == -1) return;
		glUniform3f(location, x, y, z);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& uniformName, const glm::vec4& value)
	{
		GLint location = GetUniformLocation(uniformName);
		if (location == -1) return;
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& uniformName, float x, float y, float z, float w)
	{
		GLint location = GetUniformLocation(uniformName);
		if (location == -1) return;
		glUniform4f(location, x,  y,  z, w);
	}
	void OpenGLShader::UploadUniformMat2(const std::string& uniformName, const glm::mat2& matrix)
	{
		GLint location = GetUniformLocation(uniformName);
		if (location == -1) return;
		glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void OpenGLShader::UploadUniformMat3(const std::string& uniformName, const glm::mat3& matrix)
	{
		GLint location = GetUniformLocation(uniformName);
		if (location == -1) return;
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& uniformName, const glm::mat4& matrix)
	{
		GLint location = GetUniformLocation(uniformName);
		if (location == -1) return;
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4Array(const std::string& uniformName, const glm::mat4* matrix, uint32_t count)
	{
		GLint location = GetUniformLocation(uniformName);
		if (location == -1) return;
		glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(matrix[0]));

	}
	
}