#pragma once

#include<glad/glad.h>
#include"Orion/Renderer/GraphicsCore/Texture.h"

namespace Orion 
{
	class OpenGLTexture2D : public Texture2D
	{
	public:

		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		virtual ~OpenGLTexture2D();

		virtual inline uint32_t GetWidth() const override { return m_Width; }
		virtual inline uint32_t GetHeight() const override { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;
		virtual void Bind(uint32_t slot)  override;
		virtual void Unbind(uint32_t slot) override;
		virtual bool operator==(Texture& other) override { return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID; }
		
		inline uint32_t GetCurrentSlot() override { return m_CurrentSlot; }

	private: 
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
		uint32_t m_CurrentSlot;
	};
}

