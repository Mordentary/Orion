#include "oripch.h"
#include "SubTexture.h"



namespace Orion
{
	SubTexture2D::SubTexture2D(const Shared<Texture2D>& texture, const glm::vec2& coords, const glm::uvec2& cellSize, const glm::uvec2& spriteSize ,const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture), m_CellSize(cellSize), m_SheetCoords(coords), m_SpriteSize(spriteSize)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };

		m_TexHorizontallyMirroredCoords[0] = { -min.x + 1, min.y };
		m_TexHorizontallyMirroredCoords[1] = { -max.x + 1, min.y };
		m_TexHorizontallyMirroredCoords[2] = { -max.x + 1, max.y };
		m_TexHorizontallyMirroredCoords[3] = { -min.x + 1, max.y };

	}
	Shared<SubTexture2D> SubTexture2D::CreateFromCoords(const Shared<Texture2D>& texture, const glm::vec2& coords, const glm::uvec2& cellSize, const glm::uvec2& spriteSize) 
	{

			glm::vec2 min = { (coords.x * cellSize.x) / texture->GetWidth(), (coords.y * cellSize.y) / texture->GetHeight() };
			glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(),((coords.y  + spriteSize.y) * cellSize.y) / texture->GetHeight() };


			return  CreateShared<SubTexture2D>(texture, coords, cellSize, spriteSize, min, max);

		
	}

	Shared<SubTexture2D> SubTexture2D::CreateFromCoords_Grid(const Shared<Texture2D>& texture, const glm::vec2& coords, const glm::uvec2& grid, const glm::uvec2& spriteSize)
	{
		

		uint32_t cellWidth = texture->GetWidth() / grid.x;
		uint32_t cellHeight = texture->GetHeight() / grid.y;


		glm::vec2 min = { (coords.x * cellWidth) / texture->GetWidth(), (coords.y * cellHeight) / texture->GetHeight() };
		glm::vec2 max = { ((coords.x + spriteSize.x) * cellWidth) / texture->GetWidth(),((coords.y + spriteSize.y) * cellHeight) / texture->GetHeight() };

		glm::uvec2 cellSize = { cellWidth, cellHeight };

		return  CreateShared<SubTexture2D>(texture, coords, cellSize, spriteSize, min, max);;


	}
}