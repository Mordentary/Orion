#pragma once
#include"../LightSource.h"

namespace Orion
{
	class SpotLight : public LightSource
	{
	public:
		SpotLight() 
		{
			FramebufferSpecification fb;
			fb.OnlyDepthPass = true;
			fb.CubemapBuffer = false;
			fb.Width =  512;
			fb.Height = 512;

			m_ShadowMap = Framebuffer::Create(fb);

			float nearPlane = 0.1f;
			float farPlane = 25.0f;
			m_ProjMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(fb.Width / fb.Height), nearPlane, farPlane);
		};
		SpotLight(Shared<Model> model, uint32_t shadowWidth = 512, uint32_t shadowHeight = 512, glm::vec3 pos = {0.0f,0.0f,0.0f}) : LightSource(model)
		{
			FramebufferSpecification fb;
			fb.OnlyDepthPass = true;
			fb.CubemapBuffer = false;
			fb.Width = shadowWidth;
			fb.Height = shadowHeight;

			m_ShadowMap = Framebuffer::Create(fb);

			float nearPlane = 0.1f;
			float farPlane = 100.0f;
			m_ProjMatrix = glm::perspective(glm::radians(160.f), static_cast<float>(fb.Width / fb.Height), nearPlane, farPlane);
			m_Prop.GeneralProp.Position = pos;

				//glm::ortho(-20.f, 20.f, -20.f, 20.f, -20.f, 20.f);
				//glm::perspective(glm::radians(m_OuterCutOff), static_cast<float>(fb.Width / fb.Height), nearPlane, farPlane);

		}
		

		virtual void SetupLight(Shared<Shader>& currentShader, std::vector<Shared<LightSource>>& otherLights, std::function<void()> renderFunc) override;
		virtual void LoadLightToUBO(const Shared<UniformBuffer>& ubo) override;
		virtual void LoadLightToShader(const Shared<Shader>& shader) override;
		virtual void RenderLightModel(Shared<Shader>& shader) override;

		virtual void SetLighAttenuation(float linear, float quadratic) override;
		virtual void IncreaseLightCounter() override;
		 
		virtual GeneralLightProp& GetGeneralLightProp() override { return m_Prop.GeneralProp; };
		virtual uint32_t GetLightIndex() override { return m_LightIndex; };

		inline float& GetInnerCutOff() { return m_Prop.InnerCutOff; }
		inline float& GetOuterCutOff() { return m_Prop.OuterCutOff; }

	private: 
		uint32_t m_LightIndex;
		Orion::LightSource::SpotLightProp m_Prop;
	};
}
