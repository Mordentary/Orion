#pragma once
#include"Orion/Core/Core.h"

namespace Orion{


	struct FramebufferSpecification
	{
		uint32_t Width = 0 , Height = 0;
		uint32_t Samples = 1;


		bool OnlyDepthPass = false;
		bool SwapChainTarget = false;
		bool CubemapBuffer = false;

	};

	class Framebuffer
	{
		friend class Texture2D;

		public:
			virtual ~Framebuffer() = default;

			virtual void Bind() = 0;
			virtual void Unbind() = 0;

			virtual uint32_t GetColorAttachmentID() const = 0;
			virtual uint32_t GetDepthStencilAttachmentID() const = 0;
			virtual uint32_t GetDepthAttachmentID() const = 0;

			virtual Shared<Texture2D> GetColorAttachmentTexture() const = 0;
			virtual Shared<Texture2D> GetDepthAttachmentTexture() const = 0;
			
			virtual uint32_t GetRendererID() const = 0;

			virtual void BlitToDefaultBuffer() = 0;
			virtual void BlitToBuffer(Orion::Shared<Framebuffer>& fb) = 0;


			virtual const FramebufferSpecification& GetFramebufferSpec() const = 0;
			virtual FramebufferSpecification& GetFramebufferSpec() = 0 ;



			virtual void Resize(uint32_t width, uint32_t height, bool generate_depth_renderbuffer = false) = 0;
			virtual void ClearFBOTexture(int32_t index, int value) = 0;

			static Scoped<Framebuffer> Create(const FramebufferSpecification& spec);

		


	};
						

}

