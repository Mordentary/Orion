#pragma once
#include <functional>
namespace Orion
{


	class LightSource;
	class UniformBuffer;
	class Shader;



	class LightHandler
	{
	public:
		LightHandler() = default;
		void AddLightSource(const Shared<LightSource>& light);
		void PrepareLights(Shared<Shader>& currentShader, std::function<void()> renderFunc);

		void LoadLightsToUBO(const Shared<UniformBuffer>& ubo);

		void LoadLightsToShaderAndRender(const Shared<Shader>& shader);
		void LoadLightsToShader(const Shared<Shader>& shader);
		void RenderLights();


		
		void UpdateLightSettings(float linearAttenuation, float quadraticAttenuation);
		void SetLightShader(const Shared<Shader>& shader) { m_LightShader = shader; }

		const Shared<Shader>& GetLightShader() { return m_LightShader; }
		const std::vector<Shared<LightSource>>& GetLights() { return m_LightSources; }
		uint32_t& GetDirLightsCount() { return m_DirLightCount; }
		uint32_t& GetSpotLightsCount() { return m_SpotLightCount; }
		uint32_t& GetPointLightsCount() { return m_PointLightCount; }


	private:
		uint32_t m_DirLightCount = 0 , m_PointLightCount = 0, m_SpotLightCount = 0;
		Shared<Shader> m_LightShader = nullptr;
		std::vector<Shared<LightSource>> m_LightSources;
		

	};

}

