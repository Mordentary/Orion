#pragma once


#include<string>
#include"Orion/Core/Core.h"
namespace Orion 
{


		class Texture
		{
		public: 

			virtual ~Texture() = default;

			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;

			virtual void SetData(void* data, uint32_t size) = 0;

			virtual void Bind(uint32_t slot = 1)  = 0;
			virtual void Unbind()  = 0;
			virtual uint32_t GetRendererID() const = 0;
			virtual uint32_t& GetRendererID() = 0;

			virtual uint32_t GetCurrentSlot() const = 0;

			virtual bool operator==(Texture& other) = 0;
		};



		struct Texture2DParameters
		{
			bool Flip = true;
			bool sRGBA = false;
			int32_t TEXTURE_WRAP	   = 0x2901; // DEFAULT VALUE REPRESENTS GL_REPEAT
			int32_t TEXTURE_FILTER_MIN = 0x2601; // DEFAULT VALUE REPRESENTS GL_LINEAR
			int32_t TEXTURE_FILTER_MAX = 0x2601; // DEFAULT VALUE REPRESENTS GL_LINEAR
			//TODO: MIPMAP ABSTRACTION

			Texture2DParameters() = default;
			Texture2DParameters(bool flip, bool sRGBA) : Flip(flip), sRGBA(sRGBA)
			{}
		};


		class Framebuffer;
		
		class Texture2D : public Texture 
		{
		public: 
			virtual ~Texture2D() = default;
			static Shared<Texture2D> Create(const std::string& path, const Texture2DParameters& spec = {});
			static Shared<Texture2D> Create(uint32_t width, uint32_t height);
			static Shared<Texture2D> Create(uint32_t width, uint32_t height, uint32_t samples, bool depthTexture = false, bool sRGB = false);


			static Shared<Texture2D> CreateCubemap(const std::vector<std::string>& paths);
			static Shared<Texture2D> CreateCubemap(uint32_t width, uint32_t height, bool depthCubemap = false);




		};
		
}
