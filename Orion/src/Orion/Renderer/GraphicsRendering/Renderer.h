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
		static void Init();
		static void BeginScene(const Shared<DummyCamera>& camera, const Shared<Framebuffer>& screenFB, std::function<void()> renderFunc);
		static void EndScene();

		static void DrawScene();

		static void DrawCube(const glm::mat4& modelMatrix, const Material& material);
		static void DrawSphere(const glm::mat4& modelMatrix, const Material& material);
		static void DrawModel(const glm::mat4&  modelMatrix, const Shared<Model>& model);

		static void SetSceneCubemap(const Shared<Texture2D>& cubeMap);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void AddLightToScene(const Shared<LightSource>& light);
		static void AddModelToScene(const Shared<Model>& model);


		static void LightSettings(float LinearAttenuation = 0.045, float QuadraticAttenuation = 0.0075);
	private:
		friend struct RendererData3D;
		static RendererData3D s_RenData3D;
		static void ClosestObjectToRayHit();
		static void PrepareLights();
		static void LoadAndRenderLights();
		static void LoadCubemap(const Shared<DummyCamera>& camera);
		
	};
	
}

