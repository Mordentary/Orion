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


			virtual uint32_t GetRendererID() const override { return m_RendererID; };

			virtual uint32_t GetColorAttachmentID(uint32_t index) const override { return m_ColorAttachments[index]->GetRendererID(); }
			virtual uint32_t GetDepthAttachmentID() const override { return m_DepthAttachment->GetRendererID(); }
			virtual uint32_t GetDepthStencilAttachmentID() const override { return m_DepthStencilAttachment; }

			virtual Shared<Texture2D> GetColorAttachmentTexture(uint32_t index) const override { return m_ColorAttachments[index]; }
			virtual Shared<Texture2D> GetDepthAttachmentTexture() const override { return m_DepthAttachment; }

			virtual void BlitToDefaultBuffer() override;
			virtual void BlitColorToBuffer(Orion::Shared<Framebuffer>& fb, uint32_t thisFBColorAttachIndex , uint32_t colorAttachIndex ) override;
			virtual void BlitDepthToBuffer(Orion::Shared<Framebuffer>& fb) override;
			virtual void OpenGLFramebuffer::BlitStencilToBuffer(Orion::Shared<Framebuffer>& fb) override;


			virtual void ActivateDrawingToColorTexture(uint32_t index) override;
			virtual void ActivateDrawingToAllTextures() override;

			virtual const FramebufferSpecification& GetFramebufferSpec() const { return m_Specification; }
			virtual FramebufferSpecification& GetFramebufferSpec() { return m_Specification; }

			virtual void Resize(uint32_t width, uint32_t height) override;




		private:
			FramebufferSpecification m_Specification;

			std::vector<Shared<Texture2D>> m_ColorAttachments;
			Shared<Texture2D> m_DepthAttachment = nullptr;

			uint32_t m_DepthStencilAttachment = 0;


			uint32_t m_RendererID = 0;

			void Invalidate();
			void Invalidate(const FramebufferSpecification& spec);
	};

}

