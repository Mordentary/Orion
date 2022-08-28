#include "oripch.h"
#include "AnimationLibrary.h"


namespace Orion
{
	std::unordered_map<std::string, Shared<AnimationSource>> AnimationLibrary::s_Library;

	Shared<AnimationSource> AnimationLibrary::Create(const std::string& animName, Shared<Texture2D>& texture, uint32_t frameCount, glm::uvec2& cellSize, glm::uvec2& spriteSize, glm::uvec2& sheetCoords)
	{
		auto animSrc = CreateShared<AnimationSource>(animName, frameCount);

		animSrc->CellSize = cellSize;
		animSrc->TotalFrames = frameCount;
		animSrc->Texture = texture;

		uint32_t Rows = texture->GetHeight() / cellSize.y;
		uint32_t Columns = texture->GetWidth() / cellSize.x;

		uint32_t x = sheetCoords.x;
		uint32_t y = sheetCoords.y;



		animSrc->Frames[0] = SubTexture2D::CreateFromCoords
		(
			texture,
			glm::uvec2(x, y),
			cellSize,
			spriteSize
		);


		for (size_t i = 1; i < frameCount; i++)
		{


			if (++x > Columns) {
				x = 0;
				--y;
			}


			animSrc->Frames[i] = SubTexture2D::CreateFromCoords
			(
				texture,
				glm::uvec2(x, y),
				cellSize,
				spriteSize
			);


		}
		s_Library[animName] = animSrc;

		return animSrc;
	}


	Shared<AnimationSource> AnimationLibrary::Create(const std::string& animName, Shared<Texture2D>& texture, uint32_t frameCount, glm::uvec2& cellSize, glm::uvec2& spriteSize)
	{
		auto animSrc = CreateShared<AnimationSource>(animName, frameCount);

		animSrc->CellSize = cellSize;
		animSrc->TotalFrames = frameCount;
		animSrc->Texture = texture;

		uint32_t Rows = texture->GetHeight() / cellSize.y;
		uint32_t Columns = texture->GetWidth() / cellSize.x;

		uint32_t x = 0;
		uint32_t y = Rows;



		animSrc->Frames[0] = SubTexture2D::CreateFromCoords
		(
			texture,
			glm::uvec2(x, y),
			animSrc->CellSize,
			spriteSize
		);


		for (size_t i = 1; i < frameCount; i++)
		{


			if (++x > Columns) {
				x = 0;
				--y;
			}


			animSrc->Frames[i] = SubTexture2D::CreateFromCoords
			(
				texture,
				glm::uvec2(x, y),
				cellSize,
				spriteSize
			);


		}
		s_Library[animName] = animSrc;

		return animSrc;
	}
	Shared<AnimationSource> AnimationLibrary::CreateFromGrid(const std::string& animName, Shared<Texture2D>& texture, uint32_t frameCount, glm::uvec2& columnsAndRows)
	{
		auto animSrc = CreateShared<AnimationSource>(animName, frameCount);


		animSrc->CellSize = { texture->GetWidth() / columnsAndRows.x,texture->GetHeight() / columnsAndRows.y, };
		animSrc->TotalFrames = frameCount;
		animSrc->Texture = texture;


		uint32_t x = 0;
		uint32_t y = columnsAndRows.y;



		animSrc->Frames[0] = SubTexture2D::CreateFromCoords
		(
			texture,
			glm::uvec2(x, y),
			animSrc->CellSize,
			{ 1,1 }
		);


		for (size_t i = 1; i < frameCount; i++)
		{


			if (++x > columnsAndRows.x) {
				x = 0;
				--y;
			}


			animSrc->Frames[i] = SubTexture2D::CreateFromCoords
			(
				texture,
				glm::uvec2(x, y),
				animSrc->CellSize,
				{ 1,1 }
			);


		}
		s_Library[animName] = animSrc;

		return animSrc;
	}
	Shared<AnimationSource> AnimationLibrary::CreateFromGrid(const std::string& animName, Shared<Texture2D>& texture, uint32_t frameCount, glm::uvec2& columnsAndRows, glm::uvec2& sheetCoords)
	{
		auto animSrc = CreateShared<AnimationSource>(animName, frameCount);


		animSrc->CellSize = { texture->GetWidth() / columnsAndRows.x,texture->GetHeight() / columnsAndRows.y, };
		animSrc->TotalFrames = frameCount;
		animSrc->Texture = texture;


		uint32_t x = sheetCoords.x;
		uint32_t y = sheetCoords.y;


		animSrc->Frames[0] = SubTexture2D::CreateFromCoords
		(
			texture,
			glm::uvec2(x, y),
			animSrc->CellSize,
			{ 1,1 }
		);

		for (size_t i = 1; i < frameCount; i++)
		{


			if (++x > columnsAndRows.x) {
				x = 0;
				--y;
			}


			animSrc->Frames[i] = SubTexture2D::CreateFromCoords
			(
				texture,
				glm::uvec2(x, y),
				animSrc->CellSize,
				{ 1,1 }
			);


		}
		s_Library[animName] = animSrc;
		return animSrc;
	}
	Shared<AnimationSource> AnimationLibrary::Create(const std::string& animName, Shared<SubTexture2D>& subTexture, uint32_t frameCount)
	{
		auto animSrc = CreateShared<AnimationSource>(animName, frameCount);


		animSrc->CellSize = subTexture->GetCellSize();
		animSrc->TotalFrames = frameCount;
		animSrc->Texture = subTexture->GetTexture();

		uint32_t Rows = subTexture->GetTexture()->GetHeight() / animSrc->CellSize.y;
		uint32_t Columns = subTexture->GetTexture()->GetWidth() / animSrc->CellSize.x;

		glm::uvec2 sheetCoords = subTexture->GetSheetCoords();

		animSrc->Frames[0] = subTexture;

		uint32_t x = sheetCoords.x;
		uint32_t y = sheetCoords.y;

		for (size_t i = 1; i < frameCount; i++)
		{


			if (++x > Columns) {
				x = 0;
				--y;
			}

			animSrc->Frames[i] = SubTexture2D::CreateFromCoords
			(
				subTexture->GetTexture(),
				glm::uvec2(x, y),
				subTexture->GetCellSize(),
				subTexture->GetSpriteSize()
			);


		}

		
		s_Library[animName] = animSrc;

		return animSrc;
	}


	void AnimationLibrary::AddAnimationSource(const std::string& name, const Shared<AnimationSource>& src) 
	{

		ORI_CORE_ASSERT(!Exists(name), "Animation already exists!");
		s_Library[name] = src;
	}
	void AnimationLibrary::AddAnimationSource(const Shared<AnimationSource>& src) 
	{
		auto& name = src->AnimationName;
		ORI_CORE_ASSERT(!Exists(name), "Animation already exists!");
		s_Library[name] = src;
	}
	
	bool AnimationLibrary::Exists(const std::string& name) 
	{
		return s_Library.find(name) != s_Library.end();
	}

	Shared<AnimationSource> AnimationLibrary::Get(const std::string& name)
	{
		ORI_CORE_ASSERT(Exists(name), "Animation not found!");
		return s_Library[name];
	}
}
