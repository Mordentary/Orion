#pragma once

#include"RenderCommand.h"
#include"../Cameras/OrthographicCamera.h"
#include"../Cameras/PerspectiveCamera.h"
#include"../GraphicsObjects/VertexArray.h"
#include"../GraphicsObjects/Shader.h"
#include"../GraphicsObjects/Texture.h"
#include"../GraphicsObjects/SubTexture.h"
#include"../Geometry/Mesh.h"

namespace Orion {




	class Renderer
	{
	public:
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void Init();
		static void Flush();

		static void BeginScene(const Shared<DummyCamera>& camera);
		static void EndScene();
		static void AppendMesh(const Shared<Mesh> mesh, const glm::mat4& modelMatrix);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		static void ResetBatch();
		static int32_t GetTextureSlot(const Shared<Texture2D>& textures);
		struct SceneData
		{
			static const uint32_t MaxPolygons = 20000;
			static const uint32_t MaxVertices = MaxPolygons * 3;
			static const uint32_t MaxTextureSlots = 32;

			uint32_t MeshCount = 0;
			uint32_t MeshVertexDataOffset = 0;
			uint32_t MeshIndexkoDataOffset = 0;
			uint32_t DrawCalls;
			uint32_t TotalSizeOfBatch;

			Shared<VertexArray> MeshVertexArray;
			Shared<IndexBuffer> MeshIndexBuffer;
			Shared<VertexBuffer> MeshVertexBuffer;

			Shared<Texture2D> WhiteTexture;

			Shared<Mesh>* MeshBufferBase = nullptr;
			Shared<Mesh>* MeshIterator = nullptr;


			std::array<Shared<Texture2D>, MaxTextureSlots> TextureSlots;
			uint32_t TextureSlotsIndex = 1;

			Scoped<Shader> PhongShader;

		};	
		
		static SceneData s_SceneData;
	};
}

