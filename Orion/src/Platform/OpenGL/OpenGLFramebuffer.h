#pragma once
#include"Orion/Renderer/GraphicsCore/Framebuffer.h"
#include"Orion/Renderer/GraphicsCore/Texture.h"


namespace Orion 
{

	class OpenGLFramebuffer :  public Framebuffer
	{
		public: 


			OpenGLFramebuffer(const FramebufferSpecification& spec);
			virtual ~OpenGLFramebuffer();
			virtual void Bind() override;
			virtual void Unbind() override;

			
			//virtual Shared<Texture2D> GetColorAttachment(uint32_t index) {}
			//virtual Shared<Texture2D> GetDepthS_Attachment(uint32_t index) {}

			virtual uint32_t GetColorAttachmentID()const override { return m_ColorAttachment; }

			virtual const FramebufferSpecification& GetFramebufferSpec() const { return m_Specification; }
			virtual FramebufferSpecification& GetFramebufferSpec() { return m_Specification; }

			virtual void Resize(uint32_t width, uint32_t height, bool generate_depth_renderbuffer = false) override;
			virtual void ClearFBOTexture(int32_t index, int value) override;

			void Invalidate();
			void Invalidate(const FramebufferSpecification& spec);



		private:
			uint32_t m_ColorAttachment = 0;         //TODO: MULTIPLE ATTACHMENTS DOESNT WORK!!
			uint32_t m_DepthStencilAttachment = 0;

			//uint32_t m_ColorAttachIndex = 0, m_DS_AttachIndex = 0;

			uint32_t m_RendererID = 0;
			FramebufferSpecification m_Specification;
	};

}

