#pragma once


#include<string>
#include"Orion/Core/Core.h"
namespace Orion {


		class Texture
		{
		public: 

			virtual ~Texture() = default;

			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;

			virtual void SetData(void* data, uint32_t size) = 0;

			virtual void Bind(uint32_t slot = 0)  = 0;
			virtual void Unbind(uint32_t slot = 0)  = 0;
			virtual uint32_t GetRendererID() const = 0;
			virtual uint32_t GetCurrentSlot() const = 0;

			virtual bool operator==(Texture& other) = 0;
		};

		class Texture2D : public Texture 
		{
		public: 
			static Shared<Texture2D> Create(uint32_t width, uint32_t height);
			static Shared<Texture2D> Create(const std::string& path);

		};
}
