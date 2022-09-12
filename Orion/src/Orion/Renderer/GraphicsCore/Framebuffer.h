#pragma once


namespace Orion{


	struct FramebufferSpecification
	{
		uint32_t Width, Height;

		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{

		public:


			virtual ~Framebuffer() = default;

			virtual void Bind(uint32_t width = 0, uint32_t height = 0) = 0;
			virtual void Unbind() = 0;


			virtual uint32_t GetColorAttachmentID() const = 0;

			virtual void Resize(uint32_t width, uint32_t height, bool generate_depth_renderbuffer = false) = 0;
			virtual void ClearFBOTexture(int32_t index, int value) = 0;

			static Scoped<Framebuffer> Create(const FramebufferSpecification& spec);

		private:


	};
						

}

