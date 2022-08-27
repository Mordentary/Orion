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
	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};


	struct RendererStorage2D
	{
		const uint32_t MaxQuads = 20000;
		const uint32_t MaxVertices_Quad = MaxQuads * 4;
		const uint32_t MaxIndices_Quad = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; 

		Scoped<Shader> QuadShader;
		Shared<VertexArray> QuadVertexArray;
		Shared<VertexBuffer> QuadVertexBuffer;
		Shared<Texture2D> WhiteTexture;

		const uint32_t MaxLines = 1000;
		const uint32_t MaxVertices_Line = MaxLines * 2;
		float LineWidth = 2.5f;

		Scoped<Shader> LineShader;
		Shared<VertexArray> LineVertexArray;
		Shared<VertexBuffer> LineVertexBuffer;


		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexIterator = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexIterator = nullptr;


		std::array<Shared<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotsIndex = 1;


		Orion::Renderer2D::Statistics Stats;
		uint32_t& DrawCalls = Stats.DrawCalls;
		uint32_t& QuadCount = Stats.QuadCount;
		uint32_t& LineCount = Stats.LineCount;

	};


	RendererStorage2D Renderer2D::s_Data2D;


	/////////////////////////////////////////////
	// Functions for batching //////////////////
	///////////////////////////////////////////

	void Renderer2D::Init()
	{
		ORI_PROFILE_FUNCTION();



		//Quad prepare

		uint32_t* quadIndices = new uint32_t[s_Data2D.MaxIndices_Quad];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data2D.MaxIndices_Quad; i+=6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		Shared<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data2D.MaxIndices_Quad);

		s_Data2D.QuadVertexArray = VertexArray::Create();
		s_Data2D.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data2D.QuadVertexBuffer = VertexBuffer::Create(s_Data2D.MaxLines * sizeof(QuadVertex));
		s_Data2D.QuadVertexBuffer->SetLayout({
			{Orion::ShaderDataType::Float3, "a_Position"},
			{Orion::ShaderDataType::Float4, "a_Color"},
			{Orion::ShaderDataType::Float2, "a_TextureCoord"},
			{Orion::ShaderDataType::Float, "a_TextureSlot"}

			});

		s_Data2D.QuadVertexArray->AddVertexBuffer(s_Data2D.QuadVertexBuffer);
		s_Data2D.QuadVertexBufferBase = new QuadVertex[s_Data2D.MaxVertices_Quad];


		//Line prepare

		s_Data2D.LineVertexArray = VertexArray::Create();
		s_Data2D.LineVertexBuffer = VertexBuffer::Create(s_Data2D.MaxVertices_Line * (sizeof(LineVertex)));
		s_Data2D.LineVertexBuffer->SetLayout
		(
			{
				{Orion::ShaderDataType::Float3, "a_Position"},
				{Orion::ShaderDataType::Float4, "a_Color"}
			}
		);
			
		s_Data2D.LineVertexArray->AddVertexBuffer(s_Data2D.LineVertexBuffer);
		s_Data2D.LineVertexBufferBase = new LineVertex[s_Data2D.MaxVertices_Line];


		//Texture prepare

		s_Data2D.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data2D.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t  samples[s_Data2D.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data2D.MaxTextureSlots; i++)
			samples[i] = i;
	

		//Shader preparing

		s_Data2D.QuadShader = Shader::Create("../Orion/src/Platform/OpenGL/DefaultShaders/QuadShader.glsl");
		s_Data2D.LineShader = Shader::Create("../Orion/src/Platform/OpenGL/DefaultShaders/LineShader.glsl");



		s_Data2D.QuadShader->Bind();
		s_Data2D.QuadShader->SetIntArray("u_Texture", &samples[0], s_Data2D.MaxTextureSlots);

		s_Data2D.TextureSlots[0] = s_Data2D.WhiteTexture;

		ResetBatch();

	}
	void Renderer2D::Shutdown()
	{
		ORI_PROFILE_FUNCTION();
		
	}
	void Renderer2D::BeginScene(const Shared<OrthographicCamera>& camera)
	{
		ORI_PROFILE_FUNCTION();

		s_Data2D.QuadShader->Bind();
		s_Data2D.QuadShader->SetMat4("u_ViewProjection", camera->GetProjectionViewMatrix());

		s_Data2D.LineShader->Bind();
		s_Data2D.LineShader->SetMat4("u_ViewProjection", camera->GetProjectionViewMatrix());

	}
	void Renderer2D::EndScene()
	{
		ORI_PROFILE_FUNCTION();


		Flush();
		ResetBatch();

	}
	void Renderer2D::Flush()
	{
		ORI_PROFILE_FUNCTION();

		if (s_Data2D.QuadIndexCount) {

			uint32_t dataSize = (uint8_t*)s_Data2D.QuadVertexIterator - (uint8_t*)s_Data2D.QuadVertexBufferBase;
			s_Data2D.QuadVertexBuffer->SetData(s_Data2D.QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data2D.TextureSlotsIndex; i++)
			{
				s_Data2D.TextureSlots[i]->Bind(i);
			}
			s_Data2D.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_Data2D.QuadVertexArray, s_Data2D.QuadIndexCount);
			s_Data2D.DrawCalls++;

			memset(s_Data2D.QuadVertexBufferBase, 0, dataSize);
			s_Data2D.QuadVertexBuffer->SetData(s_Data2D.QuadVertexBufferBase, dataSize);
		}

		if (s_Data2D.LineVertexCount) {
			
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data2D.LineVertexIterator - (uint8_t*)s_Data2D.LineVertexBufferBase);
			s_Data2D.LineVertexBuffer->SetData(s_Data2D.LineVertexBufferBase, dataSize);


			//ORI_CORE_INFO(s_Data2D.LineVertexCount);

			s_Data2D.LineShader->Bind();
			RenderCommand::SetLineWidth(s_Data2D.LineWidth);
			RenderCommand::DrawLines(s_Data2D.LineVertexArray, s_Data2D.LineVertexCount);
			s_Data2D.DrawCalls++;


			memset(s_Data2D.LineVertexBufferBase, 0, dataSize);
			s_Data2D.LineVertexBuffer->SetData(s_Data2D.LineVertexBufferBase, dataSize);
		}




	}
	void Renderer2D::ResetBatch()
	{
		s_Data2D.LineVertexIterator = s_Data2D.LineVertexBufferBase;
		s_Data2D.LineVertexCount = 0;

		s_Data2D.QuadVertexIterator = s_Data2D.QuadVertexBufferBase;
		s_Data2D.QuadIndexCount = 0;


		s_Data2D.TextureSlotsIndex = 1;
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

		if (s_Data2D.QuadIndexCount >= s_Data2D.MaxIndices_Quad)
			EndScene();
		
		const uint32_t textureSlot = 0;
		

		glm::mat4 modelMatrix =
			glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		

	

		AddQuadVertexToBatch(modelMatrix * glm::vec4(-size.x, -size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_BL, textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(size.x, -size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_BR, textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(size.x, size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_TR, textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(-size.x, size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_TL, textureSlot, color);


		s_Data2D.QuadIndexCount += 6;

		s_Data2D.QuadCount++;
		
	}


	 void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const glm::vec4 & color)
	 {
		 DrawQuad(glm::vec3(position, 0), size, color);
	 }
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (s_Data2D.QuadIndexCount >= s_Data2D.MaxIndices_Quad)
			EndScene();


		int32_t textureSlot = 0;

		AddQuadVertexToBatch(position, { -size.x,  -size.y }, Constants::TextureCoord_BL, textureSlot, color);
		AddQuadVertexToBatch(position, { size.x, -size.y }, Constants::TextureCoord_BR, textureSlot, color);
		AddQuadVertexToBatch(position, { size.x,  size.y }, Constants::TextureCoord_TR, textureSlot, color);
		AddQuadVertexToBatch(position, { -size.x, size.y }, Constants::TextureCoord_TL, textureSlot, color);


		s_Data2D.QuadIndexCount += 6;
		s_Data2D.QuadCount++;
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

		if (s_Data2D.QuadIndexCount >= s_Data2D.MaxIndices_Quad)
			EndScene();

		int32_t textureSlot = GetTextureSlot(texture);

		glm::mat4 modelMatrix =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		
	

		

		AddQuadVertexToBatch(modelMatrix * glm::vec4(-size.x, -size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_BL, textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(size.x, -size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_BR, textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(size.x, size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_TR, textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(-size.x, size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_TL, textureSlot, color);

		s_Data2D.QuadIndexCount += 6;
		s_Data2D.QuadCount++;

	}
	void Renderer2D::DrawTexturedQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D>& texture, const glm::vec4& color, const float tilling)
	{
		if (s_Data2D.QuadIndexCount >= s_Data2D.MaxIndices_Quad)
			EndScene();


		int32_t textureSlot = GetTextureSlot(texture);

		AddQuadVertexToBatch(position, { -size.x,  -size.y }, Constants::TextureCoord_BL, textureSlot, color);
		AddQuadVertexToBatch(position, { size.x, -size.y }, Constants::TextureCoord_BR, textureSlot, color);
		AddQuadVertexToBatch(position, { size.x,  size.y }, Constants::TextureCoord_TR, textureSlot, color);
		AddQuadVertexToBatch(position, { -size.x, size.y },   Constants::TextureCoord_TL, textureSlot, color);


		s_Data2D.QuadIndexCount += 6;
		s_Data2D.QuadCount++;
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

		if (s_Data2D.QuadIndexCount >= s_Data2D.MaxIndices_Quad)
			EndScene();

		int32_t textureSlot = GetTextureSlot(subTexture->GetTexture());

		glm::mat4 modelMatrix =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		const glm::vec2* textureCoords = subTexture->GetTextureCoords();




		AddQuadVertexToBatch(modelMatrix * glm::vec4(-size.x, -size.y, 0.0f, 1.0f), { 0,0 }, textureCoords[0], textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(size.x, -size.y, 0.0f, 1.0f), { 0,0 }, textureCoords[1], textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(size.x, size.y, 0.0f, 1.0f), { 0,0 }, textureCoords[2], textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(-size.x, size.y, 0.0f, 1.0f), { 0,0 }, textureCoords[3], textureSlot, color);

		s_Data2D.QuadIndexCount += 6;
		s_Data2D.QuadCount++;

	}
	void Renderer2D::DrawTexturedQuad(const glm::vec3& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const glm::vec4& color, const bool flip)
	{
		if (s_Data2D.QuadIndexCount >= s_Data2D.MaxIndices_Quad)
			EndScene();


		int32_t textureSlot = GetTextureSlot(subTexture->GetTexture());

		const glm::vec2* textureCoords  = subTexture->GetTextureCoords();

		if (flip)
		{
			textureCoords = subTexture->GetHorizontallyMirroredCoords();
		}



		AddQuadVertexToBatch(position, { -size.x, -size.y },  textureCoords[0], textureSlot, color);
		AddQuadVertexToBatch(position, { size.x, -size.y },	  textureCoords[1], textureSlot, color);
		AddQuadVertexToBatch(position, { size.x,  size.y },   textureCoords[2], textureSlot, color);
		AddQuadVertexToBatch(position, { -size.x, size.y },   textureCoords[3], textureSlot, color);


		s_Data2D.QuadCount++;
		s_Data2D.QuadIndexCount += 6;
	}
	void Renderer2D::DrawTexturedQuad(const glm::vec2& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const glm::vec4& color, const bool flip)
	{
		DrawTexturedQuad(glm::vec3(position, 0), size, subTexture, color, flip);
	}


	/////////////////////////////////////////////
	// Draw line  //////////////////////////////
	///////////////////////////////////////////

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
	{
		if (s_Data2D.LineVertexCount >= s_Data2D.MaxVertices_Line)
			EndScene();


		AddLineVertexToBatch(p0, color);
		AddLineVertexToBatch(p1, color);

		s_Data2D.LineCount++;
		s_Data2D.LineVertexCount += 2;
	}

	void Renderer2D::DrawBorderedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		if(s_Data2D.LineVertexCount >= s_Data2D.MaxVertices_Line)
			EndScene();


		AddLineVertexToBatch((position + glm::vec3(-size.x, size.y, 0.0f)), color);
		AddLineVertexToBatch((position + glm::vec3(size.x, size.y, 0.0f)), color);

		AddLineVertexToBatch((position + glm::vec3(size.x, size.y, 0.0f)), color);
		AddLineVertexToBatch((position + glm::vec3(size.x, -size.y, 0.0f)), color);

		AddLineVertexToBatch((position + glm::vec3(size.x, -size.y, 0.0f)), color);
		AddLineVertexToBatch((position + glm::vec3(-size.x, -size.y, 0.0f)), color);

		AddLineVertexToBatch((position + glm::vec3(-size.x, -size.y, 0.0f)), color);
		AddLineVertexToBatch((position + glm::vec3(-size.x, size.y, 0.0f)), color);

		s_Data2D.LineCount+=4;
		s_Data2D.LineVertexCount += 8;
	}










	/////////////////////////////////////////////
	// Helper func  ////////////////////////////
	///////////////////////////////////////////




	inline void Renderer2D::AddQuadVertexToBatch(const glm::vec3& position, const glm::vec2& size, const glm::vec2& textureCoord, const uint32_t textureSlot, const glm::vec4& color)
	{
		s_Data2D.QuadVertexIterator->Position = { position.x + size.x, position.y + size.y, 0.0f };;
		s_Data2D.QuadVertexIterator->Color = color;
		s_Data2D.QuadVertexIterator->TextureCoord = textureCoord;
		s_Data2D.QuadVertexIterator->TextureSlot = textureSlot;
		s_Data2D.QuadVertexIterator++;
	}
	inline void Renderer2D::AddLineVertexToBatch(const glm::vec3& point, const glm::vec4& color)
	{
		s_Data2D.LineVertexIterator->Position = { point.x, point.y , 0.0f };;
		s_Data2D.LineVertexIterator->Color = color;
		s_Data2D.LineVertexIterator++;
	}


	int32_t Renderer2D::GetTextureSlot(const Shared<Texture2D>& texture)
	{
		int32_t textureIndex = 0;
		for (int32_t i = 1; i < s_Data2D.TextureSlotsIndex; i++)
		{
			if ((*s_Data2D.TextureSlots[i].get()) == (*texture.get()))
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0)
		{
			textureIndex = s_Data2D.TextureSlotsIndex;
			s_Data2D.TextureSlots[textureIndex] = texture;
			s_Data2D.TextureSlotsIndex++;
		}
		return textureIndex;
	}


	/////////////////////////////////////////////
	// Render Statistic  ///////////////////////
	///////////////////////////////////////////


	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data2D.Stats;
	}
	void Renderer2D::ResetStats()
	{
		memset(&s_Data2D.Stats, 0, sizeof(Statistics));
	}
}