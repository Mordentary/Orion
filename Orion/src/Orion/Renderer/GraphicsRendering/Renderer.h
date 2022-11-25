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
			struct PostProcessSpec //Using this order is important to dataCopy
			{
				int32_t BloomEnable = false;
				int32_t GammaCorrectionEnable = true;
				int32_t HDR_Enable = true;

				float ReinhardWhitePoint = 1.0f;
				float Exposure = 1.0f;
				float GammaFactor = 2.2f;

				int32_t HDR_CurrentModel = 0;
				//End Uniform buffer

				bool EnableCubemap = true;
				uint32_t CubemapIndex = 0;

				uint32_t NumberBlurPasses = 10;
				float GaussianCurve[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };
			};

	public:
		static void Init();
		static void BeginScene(const Shared<DummyCamera>& camera, const Shared<Framebuffer>& finalFramebuffer, std::function<void()> renderFunc);
		static void EndScene();

		static void DrawScene();
		static void DrawCube(const glm::mat4& modelMatrix, const Material& material);
		static void DrawSphere(const glm::mat4& modelMatrix, const Material& material);
		static void DrawModel(const glm::mat4&  modelMatrix, const Shared<Model>& model);


		static void UpdateLightSettings(float LinearAttenuation = 0.045, float QuadraticAttenuation = 0.0075);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void AddSceneCubemap(const Shared<Texture2D>& cubeMap);
		static void AddLightToScene(const Shared<LightSource>& light);

		static void AddCubeToScene(const glm::mat4& modelMatrix, const Material& material);
		static void AddSphereToScene(const glm::mat4& modelMatrix, const Material& material);
		static void AddModelToScene(const Shared<Model>& model);

		static void PostProcessing(const PostProcessSpec& spec);

		static size_t GetSceneCubemapCount();
		static const Shared<Model>& GetSelectedModel();
		
		static bool& IsPipelineDeferred();

	private:
		friend struct RendererData3D;
		static RendererData3D s_RenData3D;
		static void GeometryPass();
		static void LightingPass();

		static void ClosestObjectToRayHit();
		static void PrepareLights();
		static void LoadAndRenderLights(const Shared<Shader>& shader);
		static void LoadLightsToShader(const Shared<Shader>& shader);
		static void RenderLights();

		static void DrawCubemap();



	////////////////////////
	/////////STATS/////////
	//////////////////////

	public:

		struct Statistics3D
		{
		public:
			float GetTotalTimePostProcessPass() { return m_PostProcessStageTimer.GetTimeSeconds(); }
			float GetTotalTimeLightPass() { return m_LightPassTime.GetTimeSeconds(); }
			float GetTotalTimeShadowMappingPass() { return m_ShadowMappingPass.GetTimeSeconds(); }

			friend class Renderer;
		private:
			Orion::Timer  m_PostProcessStageTimer;
			Orion::Timer  m_ShadowMappingPass;


			Orion::Timer  m_LightPassTime;
			Orion::Timer  m_GeometryPassTime;
			//Add more

		};

		static Statistics3D& GetStats();
		//static void ResetStats();


	};
	
}

