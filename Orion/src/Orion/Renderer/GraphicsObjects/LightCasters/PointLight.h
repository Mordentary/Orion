#pragma once
#include"../LightSource.h"
namespace Orion
{
	class PointLight : public LightSource
	{
	public:
		PointLight()
		{
			FramebufferSpecification fb;
			fb.OnlyDepthPass = true;
			fb.CubemapBuffer = true;
			fb.Width = 1024;
			fb.Height = 1024;

			m_ShadowMap = Framebuffer::Create(fb);

			float nearPlane = 0.1f;
			float farPlane = 25.0f;
			m_Prop.FarPlane = farPlane;
			m_ProjMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(fb.Width / fb.Height), nearPlane, farPlane);

		}
		PointLight(Shared<Model> model, uint32_t shadowWidth = 512, uint32_t shadowHeight = 512, glm::vec3 pos = {0.f,0.f,0.f }) : LightSource(model)
		{
			FramebufferSpecification fb;
			fb.OnlyDepthPass = true;
			fb.CubemapBuffer = true;
			fb.Width = shadowWidth;
			fb.Height = shadowHeight;
			
			m_ShadowMap = Framebuffer::Create(fb);
			float nearPlane = 0.1f;
			float farPlane = 25.0f;
			m_Prop.FarPlane = farPlane;
			m_ProjMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(fb.Width / fb.Height), nearPlane, farPlane);
			m_Prop.GeneralProp.Position = pos;

		}


		virtual void RenderLightModel(Shared<Shader>& shader) override;
		virtual void SetupLight(Shared<Shader>& currentShader, std::vector<Shared<LightSource>>& otherLights, std::function<void()> renderFunc) override;
		virtual void LoadLightToUBO(const Shared<UniformBuffer>& ubo) override;
		virtual void LoadLightToShader(const Shared<Shader>& shader) override;
		virtual	void UpdateFrustum() override { }

		virtual void SetLighAttenuation(float linear, float quadratic) override;
		virtual void IncreaseLightCounter() override;

		virtual uint32_t GetLightIndex() override { return m_LightIndex; };
		virtual GeneralLightProp& GetGeneralLightProp() override { return m_Prop.GeneralProp; };
		virtual LightCasterTypes GetLightType() override { return LightCasterTypes::PointLight; }


	private:

		uint32_t m_LightIndex;
		Orion::LightSource::PointLightProp m_Prop;
		
		void CalculateBoundingVolume();


	};
}
