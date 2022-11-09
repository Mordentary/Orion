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
			m_FarPlane = farPlane;
			m_ProjMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(fb.Width / fb.Height), nearPlane, farPlane);
			std::vector<glm::mat4> shadowTransforms;
			shadowTransforms.push_back(
				m_ProjMatrix *
				glm::lookAt(m_LightProp.Position, m_LightProp.Position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(m_ProjMatrix *
				glm::lookAt(m_LightProp.Position, m_LightProp.Position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(m_ProjMatrix *
				glm::lookAt(m_LightProp.Position, m_LightProp.Position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
			shadowTransforms.push_back(m_ProjMatrix *
				glm::lookAt(m_LightProp.Position, m_LightProp.Position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
			shadowTransforms.push_back(m_ProjMatrix *
				glm::lookAt(m_LightProp.Position, m_LightProp.Position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(m_ProjMatrix *
				glm::lookAt(m_LightProp.Position, m_LightProp.Position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
		}
		PointLight(Shared<Model> model, uint32_t shadowWidth = 512, uint32_t shadowHeight = 512) : LightSource(model)
		{
			FramebufferSpecification fb;
			fb.OnlyDepthPass = true;
			fb.CubemapBuffer = true;
			fb.Width = shadowWidth;
			fb.Height = shadowHeight;

			m_ShadowMap = Framebuffer::Create(fb);
		}


		virtual void RenderLightModel(Shared<Shader>& shader) override;
		virtual void SetupLight(Shared<Shader>& currentShader, std::vector<Shared<LightSource>>& otherLights, std::function<void()> renderFunc) override;
		virtual void LoadToLightShader() override;

	private:
		float m_FarPlane = 0.0f;

	};
}
