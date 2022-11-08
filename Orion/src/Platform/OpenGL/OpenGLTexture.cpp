#include "oripch.h"
#include "OpenGLTexture.h"
#include"Orion/Renderer/GraphicsCore/Framebuffer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Orion
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, const Texture2DParameters& spec) : m_TextureParam(spec)
	{
		
		int width  , height , channels;
		stbi_set_flip_vertically_on_load(m_TextureParam.Flip);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);


		ORI_CORE_ASSERT(data, std::string("Failed to load image!") + " STB_IMAGE_FAILURE_REASON: " + stbi_failure_reason() + "! Path: " + path);
//		ORI_CORE_INFO(stbi_failure_reason());



		m_Width = width;
		m_Height = height;


		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		if (channels == 1)
		{
			internalFormat = GL_R8;
			dataFormat = GL_RED;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		ORI_CORE_ASSERT(internalFormat & dataFormat, "Format not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);// Create ID for texture
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height); // Allocate memory for texture

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_TextureParam.TEXTURE_FILTER_MIN);//Param for texture (magnification, minification settings)
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, m_TextureParam.TEXTURE_FILTER_MAX);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, m_TextureParam.TEXTURE_WRAP);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, m_TextureParam.TEXTURE_WRAP);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);


		stbi_image_free(data);


	}
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
	{

		m_Width  = width;
		m_Height = height;

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		ORI_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);// Create ID for texture
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height); // Allocate memory for texture

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Param for texture (magnification, minification settings)
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t samples, bool depthTexture)
	{

		m_Width = width;
		m_Height = height;

	
		if (depthTexture) 
		{
			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			m_InternalFormat = GL_DEPTH_COMPONENT;
			m_DataFormat = GL_DEPTH_COMPONENT;

			return; //Exit!!
		}

		if (samples > 1)
		{

			//////////////////////////////////////////
			//////COLOR ATTACHMENT MULTISAMPLED///////
			//////////////////////////////////////////
			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, width, height, GL_TRUE);



			m_InternalFormat = GL_RGBA;
			m_DataFormat = GL_RGBA;


			

			return; //Exit!!!
		}


		//////////////////////////////
		//////COLOR ATTACHMENT///////
		////////////////////////////
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, bool depthCubemap)
	{
		if (depthCubemap) 
		{
			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
			for (unsigned int i = 0; i < 6; ++i)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}

	}


	OpenGLTexture2D::OpenGLTexture2D(const std::vector<std::string>& paths)
	{
		
		
	
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		//glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);

	
		stbi_set_flip_vertically_on_load(false);
		GLenum internalFormat = 0, dataFormat = 0;
		int width = 0, height = 0,channels = 0;
		//stbi_uc* data = stbi_load(paths[0].c_str(), &width, &height, &channels, 0);

		//if (channels == 4)
		//{
		//	internalFormat = GL_RGBA8;
		//	dataFormat = GL_RGBA;
		//}
		//if (channels == 3)
		//{
		//	internalFormat = GL_RGB8;
		//	dataFormat = GL_RGB;
		//}
		//if (channels == 1)
		//{
		//	internalFormat = GL_R8;
		//	dataFormat = GL_RED;
		//}
		//glTexStorage2D(GL_TEXTURE_CUBE_MAP, 0, internalFormat, width, height);

		for (uint32_t i = 0; i < paths.size(); i++)
		{
			stbi_uc* data = stbi_load(paths[i].c_str(), &width, &height, &channels, 0);
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			if (channels == 1)
			{
				internalFormat = GL_R8;
				dataFormat = GL_RED;
			}
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,internalFormat,width, height,0, dataFormat, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				ORI_CORE_ASSERT(data, std::string("Failed to load image!") + " STB_IMAGE_FAILURE_REASON: " + stbi_failure_reason() + "! Path: " + paths[i]);
				stbi_image_free(data);
			}
		}

		m_Width = width;
		m_Height = height;

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		


	}
	

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		ORI_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_RendererID);
	}
	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		ORI_PROFILE_FUNCTION();
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		ORI_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	
	}

	void OpenGLTexture2D::Unbind() 
	{
		glBindTextureUnit(m_CurrentSlot, 0);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) 
	{
		ORI_PROFILE_FUNCTION();
		m_CurrentSlot = slot;
		glBindTextureUnit(slot, m_RendererID);
	}
}