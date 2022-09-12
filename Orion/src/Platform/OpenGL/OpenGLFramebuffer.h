#pragma once
#include"Orion/Renderer/GraphicsCore/Framebuffer.h"


namespace Orion 
{

	class OpenGLFramebuffer :  public Framebuffer
	{

		public: 


			OpenGLFramebuffer(const FramebufferSpecification& spec);
			virtual ~OpenGLFramebuffer();
			virtual void Bind(uint32_t width = 0, uint32_t height = 0) override;
			virtual void Unbind() override;
			virtual uint32_t GetColorAttachmentID() const override { return m_ColorAttachment; }


			virtual void Resize(uint32_t width, uint32_t height, bool generate_depth_renderbuffer = false) override;
			virtual void ClearFBOTexture(int32_t index, int value) override;

			void Invalidate(const FramebufferSpecification& spec);



		private:
			uint32_t m_ColorAttachment, m_DepthAttachment;
			uint32_t m_RendererID;
			FramebufferSpecification m_Specification;
	};

}

