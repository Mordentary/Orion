#pragma once

#include"Orion/Renderer/GraphicsObjects/Texture.h"
#include"Orion/Renderer//GraphicsObjects/SubTexture.h"

namespace Orion {

	struct AnimationSource
	{
		AnimationSource(std::string animName, uint32_t frameCount) : AnimationName(animName), Frames(new Shared<SubTexture2D>[frameCount]), TotalFrames(frameCount) {};
		AnimationSource() = default;
		~AnimationSource()
		{
			delete[] Frames;
		}
		std::string AnimationName;
		Shared<SubTexture2D>* Frames;
		glm::uvec2 CellSize;
		uint32_t TotalFrames;
		Shared<Texture2D> Texture;

	};


	class AnimationLibrary
	{
	public:
		static Shared<AnimationSource> Create(const std::string& animName, Shared<Texture2D>& texture, uint32_t frameCount, glm::uvec2& cellSize, glm::uvec2& spriteSize, glm::uvec2& sheetCoords);
		static Shared<AnimationSource> Create(const std::string& animName, Shared<Texture2D>& texture, uint32_t frameCount, glm::uvec2& cellSize, glm::uvec2& spriteSize);

		static Shared<AnimationSource> CreateFromGrid(const std::string& animName, Shared<Texture2D>& texture, uint32_t frameCount, glm::uvec2& columnsAndRows, glm::uvec2& sheetCoords);
		static Shared<AnimationSource> CreateFromGrid(const std::string& animName, Shared<Texture2D>& texture, uint32_t frameCount, glm::uvec2& columnsAndRows);

		static Shared<AnimationSource> Create(const std::string& animName, Shared<SubTexture2D>& subTexture, uint32_t frameCount);

		static Shared<AnimationSource> Get(const std::string& name);

		void AddAnimationSource(const std::string& name, const Shared<AnimationSource>& src);
		void AddAnimationSource(const Shared<AnimationSource>& src);

		static bool Exists(const std::string& name);
		auto GetMap() { return s_Library; }

		private:
			static std::unordered_map<std::string, Shared<AnimationSource>> s_Library;
	};

}

