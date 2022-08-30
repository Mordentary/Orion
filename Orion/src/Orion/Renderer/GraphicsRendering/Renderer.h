#pragma once

#include"RenderCommand.h"
#include"../Cameras/OrthographicCamera.h"
#include"../Cameras/PerspectiveCamera.h"
#include"../GraphicsCore/VertexArray.h"
#include"../GraphicsCore/Shader.h"
#include"../GraphicsCore/Texture.h"
#include"../GraphicsCore/SubTexture.h"

namespace Orion {

class LightSource;
class Mesh;
struct Material;
class Model;

	class Renderer
	{
	public:
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void Init();
		static void Flush();

		static void BeginScene(const Shared<DummyCamera>& camera);
		static void EndScene();
		static void AddLight(const Shared<LightSource> light);
		static void AppendMesh(const Shared<Mesh> mesh, const glm::mat4& modelMatrix);
		static void DrawCube(glm::mat4 modelMatrix, const Material& material);
		static void DrawModel(glm::mat4 modelMatrix, const Shared<Model>& model);
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		friend struct RendererData3D;
		static RendererData3D s_RenData3D;
		static void LoadAllLights();
		static void ResetBatch();
		static int32_t GetTextureSlot(const Shared<Texture2D>& textures);
		
	};
}

