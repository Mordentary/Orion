#pragma once
#include"../LightSource.h"
namespace Orion
{
	class DirectionalLight : public LightSource
	{
	public:
		DirectionalLight()
		{
			FramebufferSpecification fb;
			fb.OnlyDepthPass = true;
			fb.Width = 3064;
			fb.Height = 3064;

			m_ViewMatrix = glm::lookAt(glm::vec3(0.f), m_Prop.GeneralProp.Direction, glm::vec3(0.0f, 1.0f, 0.0f));
			m_ProjMatrix = glm::ortho(-20.f, 20.f, -20.f, 20.f, -20.f, 20.f);

			m_ShadowMap = Framebuffer::Create(fb);
		};

		DirectionalLight(uint32_t shadowWidth, uint32_t shadowHeight) 
		{
			FramebufferSpecification fb;
			fb.OnlyDepthPass = true;
			fb.Width = shadowWidth;
			fb.Height = shadowHeight;

			m_ViewMatrix = glm::lookAt(glm::vec3(0.f), m_Prop.GeneralProp.Direction, glm::vec3(0.0f, 1.0f, 0.0f));
			m_ProjMatrix = glm::ortho(-20.f, 20.f, -20.f, 20.f, -20.f, 20.f);

			m_ShadowMap = Framebuffer::Create(fb);
		}

		virtual void RenderLightModel(Shared<Shader>& shader) override;
		virtual void SetupLight(Shared<Shader>& currentShader, std::vector<Shared<LightSource>>& otherLights, std::function<void()> renderFunc) override;
		virtual void LoadLightToUBO(const Shared<UniformBuffer>& ubo) override;
		virtual void LoadLightToShader(const Shared<Shader>& shader) override;

		virtual void SetLighAttenuation(float linear, float quadratic) override {};

		virtual void IncreaseLightCounter() override;
		virtual uint32_t GetLightIndex() override { return 0; };


		virtual GeneralLightProp& GetGeneralLightProp() override { return m_Prop.GeneralProp; };


	private:
		
		Orion::LightSource::DirectionalLightProp m_Prop;

	};
}
