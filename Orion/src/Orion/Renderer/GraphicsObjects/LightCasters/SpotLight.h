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
			m_FarPlane = farPlane;
			m_ProjMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(fb.Width / fb.Height), nearPlane, farPlane);
		};
		SpotLight(Shared<Model>& model, uint32_t shadowWidth = 512, uint32_t shadowHeight = 512) : LightSource(model)
		{
			FramebufferSpecification fb;
			fb.OnlyDepthPass = true;
			fb.CubemapBuffer = false;
			fb.Width = shadowWidth;
			fb.Height = shadowHeight;

			m_ShadowMap = Framebuffer::Create(fb);

			float nearPlane = 2.0f;
			float farPlane = 25.0f;
			m_FarPlane = farPlane;
			m_ProjMatrix = glm::perspective(glm::radians(m_OuterCutOff*2), static_cast<float>(fb.Width / fb.Height), nearPlane, farPlane);;

				
				//glm::ortho(-20.f, 20.f, -20.f, 20.f, -20.f, 20.f);
				//glm::perspective(glm::radians(m_OuterCutOff), static_cast<float>(fb.Width / fb.Height), nearPlane, farPlane);

		}
		

		virtual void SetupLight(Shared<Shader>& currentShader, std::vector<Shared<LightSource>>& otherLights, std::function<void()> renderFunc) override;
		virtual void LoadToLightShader() override;
		virtual void RenderLightModel(Shared<Shader>& shader) override;


		inline float& GetInnerCutOff() { return m_InnerCutOff; }
		inline float& GetOuterCutOff() { return m_OuterCutOff; }

	private: 
		float m_InnerCutOff = 20.f, m_OuterCutOff = 25.f, m_FarPlane = 0.f;
	};
}
