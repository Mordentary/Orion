#pragma once

#include"../Cameras/OrthographicCamera.h"
#include"../Cameras/PerspectiveCamera.h"
#include"../GraphicsCore/VertexArray.h"
#include"../GraphicsCore/Shader.h"
#include"../GraphicsCore/Texture.h"
#include"../GraphicsCore/SubTexture.h"



namespace Orion {

	class Renderer2D
	{
		
		
	public:
		static void Init();
		static void Shutdown();
		static void Flush();

		static void BeginScene(const Shared<DummyCamera>& camera);
		static void EndScene();

		static void AddQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color);
		static void AddQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color);
		static void AddQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void AddQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawBillboard(const Shared<Shader>& shader, const Shared<DummyCamera>& camera, glm::vec2& quadMin, glm::vec2& quadMax);



		static void AddTexturedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Shared<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), const float tilling = 1.0f);
		static void AddTexturedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Shared<Texture2D>& texture,  const glm::vec4& color = glm::vec4(1.0f), const float tilling = 1.0f);
		static void AddTexturedQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D>& texture,  const glm::vec4& color = glm::vec4(1.0f), const float tilling = 1.0f);
		static void AddTexturedQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), const float tilling = 1.0f);


		static void AddTexturedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Shared<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), const float tilling = 1.0f);
		static void AddTexturedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Shared<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), const float tilling = 1.0f);
		static void AddTexturedQuad(const glm::vec3& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), const bool flip = false);
		static void AddTexturedQuad(const glm::vec2& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const glm::vec4& color = glm::vec4(1.0f), const bool flip = false);



		static void AddLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);
		static void AddBorderedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);



	private:
		friend struct RendererData2D;
		static RendererData2D s_RenData2D;
		static void ResetBatch();
		static int32_t GetTextureSlot(const Shared<Texture2D>& textures);
		static inline void AddQuadVertexToBatch(const glm::vec3& position, const glm::vec2& size, const glm::vec2& textureCoord, const uint32_t textureSlot, const glm::vec4& color);
		static inline void AddLineVertexToBatch(const glm::vec3& p0, const glm::vec4& color);



	//Stats

	public:
		struct Statistics2D
		{
		public:
			uint32_t GetTotalDrawCalls() { return DrawCalls; }
			uint32_t GetTotalQuadCount() { return QuadCount; }
			uint32_t GetTotalLineCount() { return LineCount; }
			uint32_t GetTotalVertexCount() { return (QuadCount * 4) + (LineCount * 2); }
			uint32_t GetTotalQuadIndexCount() { return QuadCount * 6; }


			friend class Renderer2D;
		private:
			uint32_t DrawCalls;
			uint32_t QuadCount;
			uint32_t LineCount;

		};

		static Statistics2D& GetStats();
		static void ResetStats();
	};

}

