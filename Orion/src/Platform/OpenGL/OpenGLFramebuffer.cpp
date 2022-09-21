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
		/*for (size_t i = 0; i < m_ColorAttachIndex; i++)
		{
			glDeleteTextures(1, &m_ColorAttachments[i]->GetRendererID());
		}
		for (size_t i = 0; i < m_DS_AttachIndex; i++)
		{
			glDeleteTextures(1, &m_DepthStencilAttachments[i]->GetRendererID());
		}*/

		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthStencilAttachment);

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
			/*for (size_t i = 0; i < m_ColorAttachIndex; i++)
			{
				glDeleteTextures(1, &m_ColorAttachments[i]->GetRendererID());
			}
			for (size_t i = 0; i < m_DS_AttachIndex; i++)
			{
				glDeleteTextures(1, &m_DepthStencilAttachments[i]->GetRendererID());
			}*/

			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthStencilAttachment);


		}


		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);



		//////////////////////////////
		//////COLOR ATTACHMENT///////
		////////////////////////////

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);




		///////////////////////////////
		///DEPTH STENCIL ATTACHMENT///
		//////////////////////////////

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthStencilAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthStencilAttachment);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);


		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthStencilAttachment, 0);



		ORI_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is invalid");


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}