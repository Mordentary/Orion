#include "oripch.h"
#include "Renderer2D.h"
#include"RenderCommand.h"
#include "Orion/Core/Application.h"
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

	struct RendererData2D
	{
		const uint32_t MaxQuads = 20000;
		const uint32_t MaxVertices_Quad = MaxQuads * 4;
		const uint32_t MaxIndices_Quad = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Shared<Shader> QuadShader;
		Shared<VertexArray> QuadVertexArray;
		Shared<VertexBuffer> QuadVertexBuffer;
		Shared<Texture2D> WhiteTexture;

		const uint32_t MaxLines = 2000;
		const uint32_t MaxVertices_Line = MaxLines * 2;
		float LineWidth = 2.5f;

		Shared<Shader> LineShader;
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

		Orion::Renderer2D::Statistics2D Stats;
	};

	RendererData2D Renderer2D::s_RenData2D;

	/////////////////////////////////////////////
	// Functions for batching //////////////////
	///////////////////////////////////////////

	void Renderer2D::Init()
	{
		ORI_PROFILE_FUNCTION();

		//Quad preparation

		uint32_t* quadIndices = new uint32_t[s_RenData2D.MaxIndices_Quad];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_RenData2D.MaxIndices_Quad; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;

		}



		s_RenData2D.QuadVertexArray = VertexArray::Create();
		s_RenData2D.QuadVertexArray->Bind();
		Shared<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_RenData2D.MaxIndices_Quad);
		s_RenData2D.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_RenData2D.QuadVertexBuffer = VertexBuffer::Create(s_RenData2D.MaxVertices_Quad * sizeof(QuadVertex));
		s_RenData2D.QuadVertexBuffer->SetLayout({
			{Orion::ShaderDataType::Float3, "a_Position"},
			{Orion::ShaderDataType::Float4, "a_Color"},
			{Orion::ShaderDataType::Float2, "a_TextureCoord"},
			{Orion::ShaderDataType::Float, "a_TextureSlot"}
			});

		s_RenData2D.QuadVertexArray->AddVertexBuffer(s_RenData2D.QuadVertexBuffer);
		s_RenData2D.QuadVertexBufferBase = new QuadVertex[s_RenData2D.MaxVertices_Quad];

		//Line preparation

		s_RenData2D.LineVertexArray = VertexArray::Create();
		s_RenData2D.LineVertexArray->Bind();

		s_RenData2D.LineVertexBuffer = VertexBuffer::Create(s_RenData2D.MaxVertices_Line * (sizeof(LineVertex)));
		s_RenData2D.LineVertexBuffer->SetLayout
		(
			{
				{Orion::ShaderDataType::Float3, "a_Position"},
				{Orion::ShaderDataType::Float4, "a_Color"}
			}
		);

		s_RenData2D.LineVertexArray->AddVertexBuffer(s_RenData2D.LineVertexBuffer);
		s_RenData2D.LineVertexBufferBase = new LineVertex[s_RenData2D.MaxVertices_Line];

		//Texture preparation

		s_RenData2D.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_RenData2D.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t  samples[s_RenData2D.MaxTextureSlots];
		for (uint32_t i = 0; i < s_RenData2D.MaxTextureSlots; i++)
			samples[i] = i;

		//Shader preparation

		s_RenData2D.QuadShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/QuadShader.glsl");
		s_RenData2D.LineShader = ShaderLibrary::Load("../Orion/src/Platform/OpenGL/DefaultShaders/LineShader.glsl");

		s_RenData2D.QuadShader->Bind();
		s_RenData2D.QuadShader->SetIntArray("u_Texture", &samples[0], s_RenData2D.MaxTextureSlots);

		s_RenData2D.TextureSlots[0] = s_RenData2D.WhiteTexture;

		ResetBatch();
	}
	void Renderer2D::Shutdown()
	{
		ORI_PROFILE_FUNCTION();
	}
	void Renderer2D::BeginScene(const Shared<DummyCamera>& camera)
	{
		ORI_PROFILE_FUNCTION();

		s_RenData2D.QuadShader->Bind();
		s_RenData2D.QuadShader->SetMat4("u_ViewProjection", camera->GetProjectionViewMatrix());

		s_RenData2D.LineShader->Bind();
		s_RenData2D.LineShader->SetMat4("u_ViewProjection", camera->GetProjectionViewMatrix());
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

		if (s_RenData2D.QuadIndexCount) 
		{

			uint32_t dataSize = (uint32_t)((uint8_t*)s_RenData2D.QuadVertexIterator - (uint8_t*)s_RenData2D.QuadVertexBufferBase);
			s_RenData2D.QuadVertexBuffer->SetData(s_RenData2D.QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_RenData2D.TextureSlotsIndex; i++)
			{
				s_RenData2D.TextureSlots[i]->Bind(i);
			}

			s_RenData2D.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_RenData2D.QuadVertexArray, s_RenData2D.QuadIndexCount);
			s_RenData2D.Stats.DrawCalls++;

			memset(s_RenData2D.QuadVertexBufferBase, 0, dataSize);
			s_RenData2D.QuadVertexBuffer->SetData(s_RenData2D.QuadVertexBufferBase, dataSize);
		}

		if (s_RenData2D.LineVertexCount) 
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_RenData2D.LineVertexIterator - (uint8_t*)s_RenData2D.LineVertexBufferBase);
			s_RenData2D.LineVertexBuffer->SetData(s_RenData2D.LineVertexBufferBase, dataSize);

			//ORI_CORE_INFO(s_RenData2D.LineVertexCount);

			s_RenData2D.LineShader->Bind();
			RenderCommand::SetLineWidth(s_RenData2D.LineWidth);
			RenderCommand::DrawLines(s_RenData2D.LineVertexArray, s_RenData2D.LineVertexCount);
			s_RenData2D.Stats.DrawCalls++;

			memset(s_RenData2D.LineVertexBufferBase, 0, dataSize);
			s_RenData2D.LineVertexBuffer->SetData(s_RenData2D.LineVertexBufferBase, dataSize);
		}
	}
	void Renderer2D::ResetBatch()
	{
		s_RenData2D.LineVertexIterator = s_RenData2D.LineVertexBufferBase;
		s_RenData2D.LineVertexCount = 0;

		s_RenData2D.QuadVertexIterator = s_RenData2D.QuadVertexBufferBase;
		s_RenData2D.QuadIndexCount = 0;

		s_RenData2D.TextureSlotsIndex = 1;
	}

	//------------------------------------------//
	/////////////////////////////////////////////
	// RENDERING ZONE //////////////////////////
	///////////////////////////////////////////
	//--------------------------------------//

	/////////////////////////////////////////////
	// Draw simple quad  ///////////////////////
	///////////////////////////////////////////
	void Renderer2D::AddQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color)
	{
		AddQuad(glm::vec3(position, 0), size, rotation, color);
	}

	void Renderer2D::DrawBillboard(const Shared<Shader>& shader, const Shared<DummyCamera>& camera, glm::vec2& quadPosScreenCoord, glm::vec2& quadSizeScreenCoord)
	{

		auto& subScreenProp = Orion::Application::Get().GetWindow().GetSubWindowProp();
		//ORI_INFO("X: {0}, Y: {1}", mouse_x, mouse_y);
		float quadPosNDSx =  (quadPosScreenCoord.x - (subScreenProp.Width / 2.f)) / (subScreenProp.Width / 2.f);
		float quadPosNDSy = (quadPosScreenCoord.y - (subScreenProp.Height / 2.f)) / (subScreenProp.Height / 2.f);
		float z = 1.0f;

		glm::vec3 quadPosNDS = glm::vec3(quadPosNDSx, quadPosNDSy, z);

		//glm::vec4 quadPosClip = glm::vec4(quadPosNDS.x, quadPosNDS.y, -1.0, 1.0);
		//
		//glm::vec4 quadPosEye = glm::inverse(camera->GetProjectionMatrix()) * quadPosClip;
		//
		//glm::vec3 quadPosWorld = glm::vec3((inverse(camera->GetViewMatrix()) * glm::vec4(quadPosEye.x, -quadPosEye.y, -1.0, 0.0f)));

		//std::cout << glm::to_string(ray_wor) << "\n";

		float quadSizeNDSx = (quadSizeScreenCoord.x - (subScreenProp.Width/ 2.f)) / (subScreenProp.Width / 2.f);
		float quadSizeNDSy = (quadSizeScreenCoord.y - (subScreenProp.Height / 2.f)) / (subScreenProp.Height / 2.f);
	

		glm::vec3 quadSizeNDS = glm::vec3(quadSizeNDSx * 2, quadSizeNDSy * 2, z);

		//glm::vec4 quadSizeClip = glm::vec4(quadSizeNDS.x, quadSizeNDS.y, -1.0, 1.0);
		//
		//glm::vec4 quadSizeEye = glm::inverse(camera->GetProjectionMatrix()) * quadSizeClip;
		//
		//glm::vec3 quadSizeWorld = glm::vec3((inverse(camera->GetViewMatrix()) * glm::vec4(quadSizeEye.x, -quadSizeEye.y, -1.0, 0.0f)));


		glm::vec4 color{ 1.0f };
		float textureSlot = 1.0f;


		s_RenData2D.QuadVertexIterator->Position = { quadPosNDS.x ,quadPosNDS.y, 0.0f };
		s_RenData2D.QuadVertexIterator->TextureCoord = Constants::TextureCoord_BL;

		++s_RenData2D.QuadVertexIterator;

		s_RenData2D.QuadVertexIterator->Position = { quadPosNDS.x + quadSizeNDS.x,quadPosNDS.y, 0.0f };
		s_RenData2D.QuadVertexIterator->TextureCoord = Constants::TextureCoord_BR;

		++s_RenData2D.QuadVertexIterator;

		s_RenData2D.QuadVertexIterator->Position = { quadPosNDS.x + quadSizeNDS.x,quadPosNDS.y + quadSizeNDS.y, 0.0f };
		s_RenData2D.QuadVertexIterator->TextureCoord = Constants::TextureCoord_TR;

		++s_RenData2D.QuadVertexIterator;

		s_RenData2D.QuadVertexIterator->Position = { quadPosNDS.x, quadSizeNDS.y + quadPosNDS.y, 0.0f};
		s_RenData2D.QuadVertexIterator->TextureCoord = Constants::TextureCoord_TL;

		++s_RenData2D.QuadVertexIterator;

		QuadVertex* EndSegment = s_RenData2D.QuadVertexIterator;
		s_RenData2D.QuadVertexIterator -= 4;
		QuadVertex* StartSegment = s_RenData2D.QuadVertexIterator;


		uint32_t dataSize = 4*sizeof(QuadVertex);
		s_RenData2D.QuadVertexBuffer->SetData(StartSegment, dataSize);

		RenderCommand::DoDepthTest(false);
		shader->Bind();
		RenderCommand::DrawIndexed(s_RenData2D.QuadVertexArray, 6);
		s_RenData2D.Stats.DrawCalls++;

		RenderCommand::DoDepthTest(true);

		memset(StartSegment, 0, dataSize);
		s_RenData2D.QuadVertexBuffer->SetData(StartSegment, dataSize);

		shader->Unbind();
	}

	void Renderer2D::AddQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color)
	{
		ORI_PROFILE_FUNCTION();

		if (s_RenData2D.QuadIndexCount >= s_RenData2D.MaxIndices_Quad)
			EndScene();

		const uint32_t textureSlot = 0;

		glm::mat4 modelMatrix =
			glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		AddQuadVertexToBatch(modelMatrix * glm::vec4(-size.x, -size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_BL, textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(size.x, -size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_BR, textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(size.x, size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_TR, textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(-size.x, size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_TL, textureSlot, color);

		s_RenData2D.QuadIndexCount += 6;

		s_RenData2D.Stats.QuadCount++;
	}

	void Renderer2D::AddQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		AddQuad(glm::vec3(position, 0), size, color);
	}
	void Renderer2D::AddQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (s_RenData2D.QuadIndexCount >= s_RenData2D.MaxIndices_Quad)
			EndScene();

		int32_t textureSlot = 0;

		AddQuadVertexToBatch(position, { -size.x,  -size.y }, Constants::TextureCoord_BL, textureSlot, color);
		AddQuadVertexToBatch(position, { size.x, -size.y }, Constants::TextureCoord_BR, textureSlot, color);
		AddQuadVertexToBatch(position, { size.x,  size.y }, Constants::TextureCoord_TR, textureSlot, color);
		AddQuadVertexToBatch(position, { -size.x, size.y }, Constants::TextureCoord_TL, textureSlot, color);

		s_RenData2D.QuadIndexCount += 6;
		s_RenData2D.Stats.QuadCount++;
	}

	/////////////////////////////////////////////
	// Draw texture  ///////////////////////////
	///////////////////////////////////////////

	void Renderer2D::AddTexturedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Shared<Texture2D>& texture, const glm::vec4& color, const float tilling)
	{
		AddTexturedQuad(glm::vec3(position, 0), size, rotation, texture, color, tilling);
	}
	void Renderer2D::AddTexturedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Shared<Texture2D>& texture, const glm::vec4& color, const float tilling)
	{
		ORI_PROFILE_FUNCTION();

		if (s_RenData2D.QuadIndexCount >= s_RenData2D.MaxIndices_Quad)
			EndScene();

		int32_t textureSlot = GetTextureSlot(texture);

		glm::mat4 modelMatrix =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		AddQuadVertexToBatch(modelMatrix * glm::vec4(-size.x, -size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_BL, textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(size.x, -size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_BR, textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(size.x, size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_TR, textureSlot, color);
		AddQuadVertexToBatch(modelMatrix * glm::vec4(-size.x, size.y, 0.0f, 1.0f), { 0,0 }, Constants::TextureCoord_TL, textureSlot, color);

		s_RenData2D.QuadIndexCount += 6;
		s_RenData2D.Stats.QuadCount++;
	}
	void Renderer2D::AddTexturedQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D>& texture, const glm::vec4& color, const float tilling)
	{
		if (s_RenData2D.QuadIndexCount >= s_RenData2D.MaxIndices_Quad)
			EndScene();

		int32_t textureSlot = GetTextureSlot(texture);

		AddQuadVertexToBatch(position, { -size.x,  -size.y }, Constants::TextureCoord_BL, textureSlot, color);
		AddQuadVertexToBatch(position, { size.x, -size.y }, Constants::TextureCoord_BR, textureSlot, color);
		AddQuadVertexToBatch(position, { size.x,  size.y }, Constants::TextureCoord_TR, textureSlot, color);
		AddQuadVertexToBatch(position, { -size.x, size.y }, Constants::TextureCoord_TL, textureSlot, color);

		s_RenData2D.QuadIndexCount += 6;
		s_RenData2D.Stats.QuadCount++;
	}
	void Renderer2D::AddTexturedQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D>& texture, const glm::vec4& color, const float tilling)
	{
		AddTexturedQuad(glm::vec3(position, 0), size, texture, color, tilling);
	}

	/////////////////////////////////////////////
	// Draw subTexture  ////////////////////////
	///////////////////////////////////////////

	void Renderer2D::AddTexturedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Shared<SubTexture2D>& subTexture, const glm::vec4& color, const float tilling)
	{
		AddTexturedQuad(glm::vec3(position, 0), size, rotation, subTexture, color, tilling);
	}
	void Renderer2D::AddTexturedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Shared<SubTexture2D>& subTexture, const glm::vec4& color, const float tilling)
	{
		ORI_PROFILE_FUNCTION();

		if (s_RenData2D.QuadIndexCount >= s_RenData2D.MaxIndices_Quad)
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

		s_RenData2D.QuadIndexCount += 6;
		s_RenData2D.Stats.QuadCount++;
	}
	void Renderer2D::AddTexturedQuad(const glm::vec3& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const glm::vec4& color, const bool flip)
	{
		if (s_RenData2D.QuadIndexCount >= s_RenData2D.MaxIndices_Quad)
			EndScene();

		int32_t textureSlot = GetTextureSlot(subTexture->GetTexture());

		const glm::vec2* textureCoords = subTexture->GetTextureCoords();

		if (flip)
		{
			textureCoords = subTexture->GetHorizontallyMirroredCoords();
		}

		AddQuadVertexToBatch(position, { -size.x, -size.y }, textureCoords[0], textureSlot, color);
		AddQuadVertexToBatch(position, { size.x, -size.y }, textureCoords[1], textureSlot, color);
		AddQuadVertexToBatch(position, { size.x,  size.y }, textureCoords[2], textureSlot, color);
		AddQuadVertexToBatch(position, { -size.x, size.y }, textureCoords[3], textureSlot, color);

		s_RenData2D.Stats.QuadCount++;
		s_RenData2D.QuadIndexCount += 6;
	}
	void Renderer2D::AddTexturedQuad(const glm::vec2& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const glm::vec4& color, const bool flip)
	{
		AddTexturedQuad(glm::vec3(position, 0), size, subTexture, color, flip);
	}

	/////////////////////////////////////////////
	// Draw line  //////////////////////////////
	///////////////////////////////////////////

	void Renderer2D::AddLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
	{
		if (s_RenData2D.LineVertexCount >= s_RenData2D.MaxVertices_Line)
			EndScene();

		AddLineVertexToBatch(p0, color);
		AddLineVertexToBatch(p1, color);

		s_RenData2D.Stats.LineCount++;
		s_RenData2D.LineVertexCount += 2;
	}

	void Renderer2D::AddBorderedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (s_RenData2D.LineVertexCount >= s_RenData2D.MaxVertices_Line)
			EndScene();

		AddLineVertexToBatch((position + glm::vec3(-size.x, size.y, 0.0f)), color);
		AddLineVertexToBatch((position + glm::vec3(size.x, size.y, 0.0f)), color);

		AddLineVertexToBatch((position + glm::vec3(size.x, size.y, 0.0f)), color);
		AddLineVertexToBatch((position + glm::vec3(size.x, -size.y, 0.0f)), color);

		AddLineVertexToBatch((position + glm::vec3(size.x, -size.y, 0.0f)), color);
		AddLineVertexToBatch((position + glm::vec3(-size.x, -size.y, 0.0f)), color);

		AddLineVertexToBatch((position + glm::vec3(-size.x, -size.y, 0.0f)), color);
		AddLineVertexToBatch((position + glm::vec3(-size.x, size.y, 0.0f)), color);

		s_RenData2D.Stats.LineCount += 4;
		s_RenData2D.LineVertexCount += 8;
	}

	/////////////////////////////////////////////
	// Helper func  ////////////////////////////
	///////////////////////////////////////////

	inline void Renderer2D::AddQuadVertexToBatch(const glm::vec3& position, const glm::vec2& size, const glm::vec2& textureCoord, const uint32_t textureSlot, const glm::vec4& color)
	{
		s_RenData2D.QuadVertexIterator->Position = { position.x + size.x, position.y + size.y, 0.0f };;
		s_RenData2D.QuadVertexIterator->Color = color;
		s_RenData2D.QuadVertexIterator->TextureCoord = textureCoord;
		s_RenData2D.QuadVertexIterator->TextureSlot = textureSlot;
		++s_RenData2D.QuadVertexIterator;
	}
	inline void Renderer2D::AddLineVertexToBatch(const glm::vec3& point, const glm::vec4& color)
	{
		s_RenData2D.LineVertexIterator->Position = { point.x, point.y ,point.z };;
		s_RenData2D.LineVertexIterator->Color = color;
		++s_RenData2D.LineVertexIterator;
	}

	int32_t Renderer2D::GetTextureSlot(const Shared<Texture2D>& texture)
	{
		int32_t textureIndex = 0;
		for (int32_t i = 1; i < s_RenData2D.TextureSlotsIndex; i++)
		{
			if ((*s_RenData2D.TextureSlots[i].get()) == (*texture.get()))
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0)
		{
			textureIndex = s_RenData2D.TextureSlotsIndex;
			s_RenData2D.TextureSlots[textureIndex] = texture;
			s_RenData2D.TextureSlotsIndex++;
		}
		return textureIndex;
	}

	/////////////////////////////////////////////
	// Render Statistic  ///////////////////////
	///////////////////////////////////////////

	Renderer2D::Statistics2D& Renderer2D::GetStats()
	{
		return s_RenData2D.Stats;
	}
	void Renderer2D::ResetStats()
	{
		memset(&s_RenData2D.Stats, 0, sizeof(Statistics2D));
	}
}