#pragma once

#include<glad/glad.h>
#include"Orion/Renderer/GraphicsCore/Texture.h"


namespace Orion 
{

	class OpenGLTexture2D : public Texture2D
	{
	public:

		OpenGLTexture2D(const std::string& path, const Texture2DParameters& spec); //Creates texture from path
		OpenGLTexture2D(const std::vector<std::string>& paths); //Creates Cubemap from paths

		OpenGLTexture2D(uint32_t width, uint32_t height); //Creates clean RGBA texture

		OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t samples, bool depthTexture); //Creates texture for framebuffer
		OpenGLTexture2D(uint32_t width, uint32_t height, bool depthCubemap); //Creates cubemap for framebuffer //TODO: Depth cubemaps	
		


		virtual ~OpenGLTexture2D();

		virtual inline uint32_t GetWidth() const override { return m_Width; }
		virtual inline uint32_t GetHeight() const override { return m_Height; }

		virtual inline uint32_t GetRendererID() const override { return m_RendererID; }
		virtual inline uint32_t& GetRendererID() override { return m_RendererID; }

		inline uint32_t GetCurrentSlot() const override { return m_CurrentSlot; }


		virtual void SetData(void* data, uint32_t size) override;
		virtual void Bind(uint32_t slot)  override;
		virtual void Unbind() override;
		virtual bool operator==(Texture& other) override { return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID; }
		

	private: 
		std::string m_Path;
		uint32_t m_Width = 0, m_Height = 0;
		GLenum m_InternalFormat = 0, m_DataFormat = 0;
		uint32_t m_RendererID = 0;
		uint32_t m_CurrentSlot = 0;
		Texture2DParameters m_TextureParam;

		
	};

	

}

