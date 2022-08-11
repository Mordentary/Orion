#include "oripch.h"
#include "Renderer2D.h"
#include"RenderCommand.h"

namespace Orion
{


	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoord;
		float TextureSlot;
		
	};

	struct RendererStorage2D
	{
		const uint32_t MaxQuads = 1000*1000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; 

		Scoped<Shader> TextureShader;
		Shared<VertexArray> TexQuadArray;
		Shared<VertexBuffer> TexQuadBuffer;
		Shared<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* PtrToFirstVertex = nullptr;
		QuadVertex* VertexIterator = nullptr;

		std::array<Shared<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotsIndex = 1;


		Orion::Renderer2D::Statistics Stats;
		uint32_t& DrawCalls = Stats.DrawCalls;
		uint32_t& QuadCount = Stats.QuadCount;
	};


	RendererStorage2D Renderer2D::s_RendererData2D;


	/////////////////////////////////////////////
	// Functions for batching //////////////////
	///////////////////////////////////////////

	void Renderer2D::Init()
	{
		ORI_PROFILE_FUNCTION();

		//s_RendererData2D = std::make_unique<RendererStorage2D>();

		s_RendererData2D.TexQuadArray = VertexArray::Create();
		s_RendererData2D.TexQuadArray->Bind();


		uint32_t* quadIndices = new uint32_t[s_RendererData2D.MaxIndices];

		uint32_t offset = 0;
		for (size_t i = 0; i < s_RendererData2D.MaxIndices; i+=6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Shared<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_RendererData2D.MaxIndices);
		s_RendererData2D.TexQuadArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;


		s_RendererData2D.PtrToFirstVertex = new QuadVertex[s_RendererData2D.MaxVertices];

		s_RendererData2D.TexQuadBuffer = VertexBuffer::Create(s_RendererData2D.MaxVertices * sizeof(QuadVertex));

		Orion::BufferLayout layoutQuadTex =
		{
			{Orion::ShaderDataType::Float3, "a_Position"},
			{Orion::ShaderDataType::Float4, "a_Color"},
			{Orion::ShaderDataType::Float2, "a_TextureCoord"},
			{Orion::ShaderDataType::Float, "a_TextureSlot"}

		};
		s_RendererData2D.TexQuadBuffer->SetLayout(layoutQuadTex);



		s_RendererData2D.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_RendererData2D.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t  samples[32];
		for (uint32_t i = 0; i < s_RendererData2D.MaxTextureSlots; i++)
		{
			samples[i] = i;
		}
	
		s_RendererData2D.TextureShader = Shader::Create("../Orion/src/Platform/OpenGL/DefaultShaders/BatchShader.glsl");
		s_RendererData2D.TextureShader->Bind();
		s_RendererData2D.TextureShader->SetIntArray("u_Texture", &samples[0], s_RendererData2D.MaxTextureSlots);


		s_RendererData2D.TextureSlots[0] = s_RendererData2D.WhiteTexture;

		s_RendererData2D.VertexIterator = s_RendererData2D.PtrToFirstVertex;
		s_RendererData2D.QuadIndexCount = 0;
		s_RendererData2D.TextureSlotsIndex = 1;

	}
	void Renderer2D::Shutdown()
	{
		ORI_PROFILE_FUNCTION();
		
	}
	void Renderer2D::BeginScene(const Shared<OrthographicCamera>& camera)
	{
		ORI_PROFILE_FUNCTION();

		s_RendererData2D.TextureShader->Bind();
		s_RendererData2D.TextureShader->SetMat4("u_ViewProjection", camera->GetProjectionViewMatrix());

	}
	void Renderer2D::EndScene()
	{
		ORI_PROFILE_FUNCTION();

		
		uint32_t dataSize = (uint8_t*)s_RendererData2D.VertexIterator - (uint8_t*)s_RendererData2D.PtrToFirstVertex;
			
		s_RendererData2D.TexQuadBuffer->SetData(s_RendererData2D.PtrToFirstVertex, dataSize);
		s_RendererData2D.TexQuadArray->AddVertexBuffer(s_RendererData2D.TexQuadBuffer);
		Flush();
		ResetBatch(dataSize);

		
	}
	void Renderer2D::Flush()
	{
		ORI_PROFILE_FUNCTION();
		for (uint32_t  i = 0; i < s_RendererData2D.TextureSlotsIndex; i++)
		{
			s_RendererData2D.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_RendererData2D.TexQuadArray, s_RendererData2D.QuadIndexCount);
		s_RendererData2D.DrawCalls++;
	
	}
	void Renderer2D::ResetBatch(int32_t dataSize)
	{
		memset(s_RendererData2D.PtrToFirstVertex, 0, dataSize);
		s_RendererData2D.TexQuadBuffer->SetData(s_RendererData2D.PtrToFirstVertex, dataSize);
		s_RendererData2D.VertexIterator = s_RendererData2D.PtrToFirstVertex;
		s_RendererData2D.QuadIndexCount = 0;
		s_RendererData2D.TextureSlotsIndex = 1;
	}



	//------------------------------------------//
	/////////////////////////////////////////////
	// RENDERING ZONE //////////////////////////
	///////////////////////////////////////////
	//--------------------------------------//





	/////////////////////////////////////////////
	// Draw simple quad  ///////////////////////
	///////////////////////////////////////////
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position, 0), size, rotation, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color)
	{
		ORI_PROFILE_FUNCTION();

		if (s_RendererData2D.QuadIndexCount >= s_RendererData2D.MaxIndices)
			EndScene();
		
		const uint32_t textureSlot = 0;
		

		glm::mat4 modelMatrix =
			glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		

		if (s_RendererData2D.QuadIndexCount >= s_RendererData2D.MaxIndices)
			EndScene();

		AddVertexToBatch(modelMatrix * glm::vec4(-size.x, -size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_BL, textureSlot, color);
		AddVertexToBatch(modelMatrix * glm::vec4(size.x, -size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_BR, textureSlot, color);
		AddVertexToBatch(modelMatrix * glm::vec4(size.x, size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_TR, textureSlot, color);
		AddVertexToBatch(modelMatrix * glm::vec4(-size.x, size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_TL, textureSlot, color);


		s_RendererData2D.QuadIndexCount += 6;

		s_RendererData2D.QuadCount++;
		
	}


	 void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const glm::vec4 & color)
	 {
		 DrawQuad(glm::vec3(position, 0), size, color);
	 }
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (s_RendererData2D.QuadIndexCount >= s_RendererData2D.MaxIndices)
			EndScene();


		int32_t textureSlot = 0;

		AddVertexToBatch(position, { -size.x,  -size.y }, Constants::TextureCoord_BL, textureSlot, color);
		AddVertexToBatch(position, { size.x, -size.y }, Constants::TextureCoord_BR, textureSlot, color);
		AddVertexToBatch(position, { size.x,  size.y }, Constants::TextureCoord_TR, textureSlot, color);
		AddVertexToBatch(position, { -size.x, size.y }, Constants::TextureCoord_TL, textureSlot, color);


		s_RendererData2D.QuadIndexCount += 6;
		s_RendererData2D.QuadCount++;
	}



	/////////////////////////////////////////////
	// Draw texture  ///////////////////////////
	///////////////////////////////////////////

	void Renderer2D::DrawTexturedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Shared<Texture2D>& texture,  const glm::vec4& color, const float tilling)
	{

		DrawTexturedQuad(glm::vec3(position, 0), size, rotation, texture, color, tilling);
	}
	void Renderer2D::DrawTexturedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Shared<Texture2D>& texture, const glm::vec4& color, const float tilling)
	{
		ORI_PROFILE_FUNCTION();

		if (s_RendererData2D.QuadIndexCount >= s_RendererData2D.MaxIndices)
			EndScene();

		int32_t textureSlot = GetTextureSlot(texture);

		glm::mat4 modelMatrix =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		
	

		

		AddVertexToBatch(modelMatrix * glm::vec4(-size.x, -size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_BL, textureSlot, color);
		AddVertexToBatch(modelMatrix * glm::vec4(size.x, -size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_BR, textureSlot, color);
		AddVertexToBatch(modelMatrix * glm::vec4(size.x, size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_TR, textureSlot, color);
		AddVertexToBatch(modelMatrix * glm::vec4(-size.x, size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_TL, textureSlot, color);

		s_RendererData2D.QuadIndexCount += 6;
		s_RendererData2D.QuadCount++;

	}
	void Renderer2D::DrawTexturedQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D>& texture, const glm::vec4& color, const float tilling)
	{
		if (s_RendererData2D.QuadIndexCount >= s_RendererData2D.MaxIndices)
			EndScene();


		int32_t textureSlot = GetTextureSlot(texture);

		AddVertexToBatch(position, { -size.x,  -size.y }, Constants::TextureCoord_BL, textureSlot, color);
		AddVertexToBatch(position, { size.x, -size.y }, Constants::TextureCoord_BR, textureSlot, color);
		AddVertexToBatch(position, { size.x,  size.y }, Constants::TextureCoord_TR, textureSlot, color);
		AddVertexToBatch(position, { -size.x, size.y },   Constants::TextureCoord_TL, textureSlot, color);


		s_RendererData2D.QuadIndexCount += 6;
		s_RendererData2D.QuadCount++;
	}
	void Renderer2D::DrawTexturedQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D>& texture, const glm::vec4& color, const float tilling)
	{
		DrawTexturedQuad(glm::vec3(position, 0), size,  texture, color, tilling);
	}





	/////////////////////////////////////////////
	// Draw subTexture  ////////////////////////
	///////////////////////////////////////////


	void Renderer2D::DrawTexturedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Shared<SubTexture2D>& subTexture, const glm::vec4& color, const float tilling)
	{

		DrawTexturedQuad(glm::vec3(position, 0), size, rotation, subTexture, color, tilling);
	}
	void Renderer2D::DrawTexturedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Shared<SubTexture2D>& subTexture, const glm::vec4& color, const float tilling)
	{
		ORI_PROFILE_FUNCTION();

		if (s_RendererData2D.QuadIndexCount >= s_RendererData2D.MaxIndices)
			EndScene();

		int32_t textureSlot = GetTextureSlot(subTexture->GetTexture());

		glm::mat4 modelMatrix =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		const glm::vec2* textureCoords = subTexture->GetTextureCoords();




		AddVertexToBatch(modelMatrix * glm::vec4(-size.x, -size.y, 0.0f, 1.0f), { 0,0 }, textureCoords[0], textureSlot, color);
		AddVertexToBatch(modelMatrix * glm::vec4(size.x, -size.y, 0.0f, 1.0f), { 0,0 }, textureCoords[1], textureSlot, color);
		AddVertexToBatch(modelMatrix * glm::vec4(size.x, size.y, 0.0f, 1.0f), { 0,0 }, textureCoords[2], textureSlot, color);
		AddVertexToBatch(modelMatrix * glm::vec4(-size.x, size.y, 0.0f, 1.0f), { 0,0 }, textureCoords[3], textureSlot, color);

		s_RendererData2D.QuadIndexCount += 6;
		s_RendererData2D.QuadCount++;

	}
	void Renderer2D::DrawTexturedQuad(const glm::vec3& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const glm::vec4& color, const bool flip)
	{
		if (s_RendererData2D.QuadIndexCount >= s_RendererData2D.MaxIndices)
			EndScene();


		int32_t textureSlot = GetTextureSlot(subTexture->GetTexture());

		const glm::vec2* textureCoords  = subTexture->GetTextureCoords();

		if (flip)
		{
			textureCoords = subTexture->GetHorizontallyMirroredCoords();
		}



		AddVertexToBatch(position, { -size.x, -size.y },  textureCoords[0], textureSlot, color);
		AddVertexToBatch(position, { size.x, -size.y },	  textureCoords[1], textureSlot, color);
		AddVertexToBatch(position, { size.x,  size.y },   textureCoords[2], textureSlot, color);
		AddVertexToBatch(position, { -size.x, size.y },   textureCoords[3], textureSlot, color);


		s_RendererData2D.QuadIndexCount += 6;
		s_RendererData2D.QuadCount++;
	}
	void Renderer2D::DrawTexturedQuad(const glm::vec2& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const glm::vec4& color, const bool flip)
	{
		DrawTexturedQuad(glm::vec3(position, 0), size, subTexture, color, flip);
	}































	inline void Renderer2D::AddVertexToBatch(const glm::vec3& position, const glm::vec2& size, const glm::vec2& textureCoord, const uint32_t textureSlot, const glm::vec4& color)
	{
		s_RendererData2D.VertexIterator->Position = { position.x + size.x, position.y + size.y, 0.0f };;
		s_RendererData2D.VertexIterator->Color = color;
		s_RendererData2D.VertexIterator->TextureCoord = textureCoord;
		s_RendererData2D.VertexIterator->TextureSlot = textureSlot;
		s_RendererData2D.VertexIterator++;
	}


	int32_t Renderer2D::GetTextureSlot(const Shared<Texture2D>& texture)
	{
		int32_t textureIndex = 0;
		for (int32_t i = 1; i < s_RendererData2D.TextureSlotsIndex; i++)
		{
			if ((*s_RendererData2D.TextureSlots[i].get()) == (*texture.get()))
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0)
		{
			textureIndex = s_RendererData2D.TextureSlotsIndex;
			s_RendererData2D.TextureSlots[textureIndex] = texture;
			s_RendererData2D.TextureSlotsIndex++;
		}
		return textureIndex;
	}

	//Stats
	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_RendererData2D.Stats;
	}
	void Renderer2D::ResetStats()
	{
		memset(&s_RendererData2D.Stats, 0, sizeof(Statistics));
	}
}