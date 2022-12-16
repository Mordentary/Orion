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
    struct LightCounters
    {
        uint32_t PointLightCount = 0;
        uint32_t SpotLightCount = 0;
        uint32_t DirLightCount = 0;
    };

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
        const Shared<LightSource>& GetDirectionaLight() { return m_DirectionalLight; }

    

        const std::vector<Shared<LightSource>>& GetLights() { return m_LightSources; }

       // LightCounters& GetLightCounters() { return m_LightCounters; }
        uint32_t GetMaxLightsPerType() { return MAX_LIGHTS; }

        friend class PointLight;
        friend class SpotLight;
        friend class DirectionalLight;

    private:
        static LightCounters m_LightCounters;
        static const uint32_t MAX_LIGHTS = 100;
        Shared<Shader> m_LightShader = nullptr;
        std::vector<Shared<LightSource>> m_LightSources;
        Shared<LightSource> m_DirectionalLight = nullptr;

    };

}

