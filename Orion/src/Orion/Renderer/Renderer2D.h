#pragma once

#include"Orion/Renderer/OrthographicCamera.h"
#include"Orion/Renderer/VertexArray.h"
#include"Orion/Renderer/Shader.h"
#include"Texture.h"
#include"SubTexture.h"



namespace Orion {

	class Renderer2D
	{
		
		
	public:
		static void Init();
		static void Shutdown();
		static void Flush();

		static void BeginScene(const Shared<OrthographicCamera>& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);


		static void DrawTexturedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Shared<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), const float tilling = 1.0f);
		static void DrawTexturedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Shared<Texture2D>& texture,  const glm::vec4& color = glm::vec4(1.0f), const float tilling = 1.0f);
		static void DrawTexturedQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D>& texture,  const glm::vec4& color = glm::vec4(1.0f), const float tilling = 1.0f);
		static void DrawTexturedQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), const float tilling = 1.0f);


		static void DrawTexturedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Shared<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), const float tilling = 1.0f);
		static void DrawTexturedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Shared<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), const float tilling = 1.0f);
		static void DrawTexturedQuad(const glm::vec3& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), const bool flip = false);
		static void DrawTexturedQuad(const glm::vec2& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), const bool flip = false);


		//Stats
		struct Statistics
		{
		public:
			 uint32_t GetTotalDrawCalls() { return DrawCalls; }
			 uint32_t GetTotalQuadCount() { return QuadCount; }
			 uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			 uint32_t GetTotalIndexCount() { return QuadCount * 6; }

		private:
			friend struct RendererStorage2D;
			uint32_t DrawCalls;
			uint32_t QuadCount;
		};
	
		static Statistics GetStats();
		static void ResetStats();


	private:
		static RendererStorage2D s_RendererData2D;
		static void ResetBatch(int32_t dataSize);
		static inline void AddVertexToBatch(const glm::vec3& position, const glm::vec2& size, const glm::vec2& textureCoord, const uint32_t textureSlot, const glm::vec4& color);
		static int32_t GetTextureSlot(const Shared<Texture2D>& textures);
	};

}

