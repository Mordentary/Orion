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
			fb.Width = 512;
			fb.Height = 512;

			m_ShadowMap = Framebuffer::Create(fb);
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

	};
}
