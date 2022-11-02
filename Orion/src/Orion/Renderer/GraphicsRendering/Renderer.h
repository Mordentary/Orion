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
		static void AddLight(const Shared<LightSource>& light, const Shared<Model>& lightModel = nullptr);
		static void AppendMesh(const Shared<Mesh>& mesh, const glm::mat4& modelMatrix);
		static void DrawCube(const glm::mat4& modelMatrix, const Material& material);
		static void DrawSphere(const glm::mat4& modelMatrix, const Material& material);
		static void DrawModel(const glm::mat4&  modelMatrix, const Shared<Model>& model);
		static void LightSettings(float LinearAttenuation = 0.045, float QuadraticAttenuation = 0.0075);


		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		static void SetSceneCubemap(const Shared<Texture2D>& cubeMap);
	private:
		friend struct RendererData3D;
		static RendererData3D s_RenData3D;
		static void LoadLights();
		static void LoadCubemap(const Shared<DummyCamera>& camera);
		static void ResetBatch();
		static int32_t GetTextureSlot(const Shared<Texture2D>& textures);
		
	};
}

