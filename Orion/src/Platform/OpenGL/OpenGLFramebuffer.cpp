#include "oripch.h"
#include "OpenGLFramebuffer.h"
#include <glad/glad.h>

namespace Orion 
{

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) : m_Specification(spec) 
	{
		Invalidate(spec);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);

		if(m_ColorAttachment) glDeleteTextures(1, &m_ColorAttachment->GetRendererID()); m_ColorAttachment = nullptr;;
		if (m_DepthAttachment) glDeleteTextures(1, &m_DepthAttachment->GetRendererID()); m_DepthAttachment = nullptr;;

		glDeleteRenderbuffers(1, &m_DepthStencilAttachment);

	}
	inline void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);

	}
	inline void OpenGLFramebuffer::Unbind() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
	void OpenGLFramebuffer::BlitToDefaultBuffer() 
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT , GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
	void OpenGLFramebuffer::BlitToBuffer(Orion::Shared<Framebuffer>& fb)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->GetRendererID());
		glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT , GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	inline void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height, bool generate_depth_renderbuffer)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}
	inline void OpenGLFramebuffer::ClearFBOTexture(int32_t index, int value) 
	{

	}
	void OpenGLFramebuffer::Invalidate(const FramebufferSpecification& spec)
	{
		m_Specification = spec;
		Invalidate();
	}
	void OpenGLFramebuffer::Invalidate()
	{
			if(m_RendererID)
			{
				glDeleteFramebuffers(1, &m_RendererID);

				if (m_ColorAttachment) glDeleteTextures(1, &m_ColorAttachment->GetRendererID()); m_ColorAttachment = nullptr;
				if (m_DepthAttachment) glDeleteTextures(1, &m_DepthAttachment->GetRendererID()); m_DepthAttachment = nullptr;

				glDeleteRenderbuffers(1, &m_DepthStencilAttachment);
			}


			glGenFramebuffers(1, &m_RendererID);
			glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);


			if (m_Specification.OnlyDepthPass) 
			{

				if (m_Specification.CubemapBuffer) 
				{
					m_DepthAttachment = Orion::Texture2D::CreateCubemap(m_Specification.Width, m_Specification.Height, true);
					glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthAttachment->GetRendererID(), 0);
					glDrawBuffer(GL_NONE);
					glReadBuffer(GL_NONE);
					ORI_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is invalid");

					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					
		
					return;
				}
				
				m_DepthAttachment = Orion::Texture2D::Create(m_Specification.Width, m_Specification.Height, m_Specification.Samples ,true);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment->GetRendererID(), 0);
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);

				ORI_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is invalid");
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				return;
			}

			m_ColorAttachment = Orion::Texture2D::Create(m_Specification.Width, m_Specification.Height, m_Specification.Samples, false);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_Specification.Samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_ColorAttachment->GetRendererID(), 0);
		

				
			///////////////////////////////
			///RENDEROBJECT ATTACHMENT////
			//////////////////////////////

			if (m_Specification.Samples > 1)
			{

				///////////////////////////////////
				///DEPTH STENCIL MULTISAMPLED//////
				///////////////////////////////////

				glGenRenderbuffers(1, &m_DepthStencilAttachment);
				glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilAttachment);
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_Specification.Samples, GL_DEPTH32F_STENCIL8, m_Specification.Width, m_Specification.Height);

				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilAttachment);

				ORI_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is invalid:");

				glBindFramebuffer(GL_FRAMEBUFFER, 0);

			}
			else
			{

				///////////////////////////////
				///DEPTH STENCIL ATTACHMENT///
				//////////////////////////////

				glGenRenderbuffers(1, &m_DepthStencilAttachment);
				glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilAttachment);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, m_Specification.Width, m_Specification.Height);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);

				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilAttachment);

				ORI_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is invalid");

				glBindFramebuffer(GL_FRAMEBUFFER, 0);

			}
	}
}