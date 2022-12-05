#pragma once
#include"../GraphicsCore/Texture.h"
#include<algorithm>
namespace Orion
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Shared<Texture2D>& texture, const glm::vec2& coords, const glm::uvec2& cellSize, const glm::uvec2& spriteSize, const glm::vec2& min, const glm::vec2& max);


		 static Shared<SubTexture2D> CreateFromCoords(const Shared<Texture2D>& texture, const glm::vec2& coords, const glm::uvec2& cellSize, const glm::uvec2& spriteSize = glm::vec2(1.0f));
		 static Shared<SubTexture2D> CreateFromCoords_Grid(const Shared<Texture2D>& texture, const glm::vec2& coords, const glm::uvec2& grid, const glm::uvec2& spriteSize = glm::vec2(1.0f));


			inline const glm::vec2* GetTextureCoords() const { return m_TexCoords; }
			inline const glm::vec2* GetHorizontallyMirroredCoords() const {return  m_TexHorizontallyMirroredCoords;}
			inline const  Shared<Texture2D>& GetTexture() const { return m_Texture; }
			inline const glm::uvec2& GetCellSize() const { return m_CellSize; }
			inline const glm::uvec2& GetSpriteSize() const { return m_SpriteSize; }
			inline const glm::uvec2& GetSheetCoords() const { return m_SheetCoords; }

	private: 
		Shared<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
		glm::vec2 m_TexHorizontallyMirroredCoords[4];
		glm::uvec2 m_CellSize;
		glm::uvec2 m_SpriteSize;
		glm::uvec2 m_SheetCoords;
	};
}
