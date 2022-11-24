#pragma once
#include"Model.h"
#include"../GraphicsCore/Shader.h"
#include"../GraphicsCore/Framebuffer.h"
namespace Orion
{

	struct LightProperties
	{
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Direction{ 0.0f,0.0f, -1.0f };

		glm::vec3 AmbientLightColor{ 0.01f, 0.01f, 0.01f };
		glm::vec3 DiffuseLightColor{ 0.9f, 0.9f, 0.9f};
		glm::vec3 SpecularLightColor{ 0.5f, 0.5f, 0.5f };

		float ConstantAttenuation =  1.0f;
		float LinearAttenuation =  0.045;
		float QuadraticAttenuation =  0.0075;
	};
	class LightSource 
	{
	public:

		LightSource() = default;
		virtual ~LightSource() = default;


		LightSource(const Shared<Model>& lightModel) : m_LightModel(lightModel)
		{
			
		}
		virtual void LoadToLightShader(const Shared<Shader>& shader) = 0;
		virtual void SetupLight(Shared<Shader>& currentShader, std::vector<Shared<LightSource>>& otherLights, std::function<void()> renderFunc) = 0;
		virtual void RenderLightModel(Shared<Shader>& shader) = 0;


		const Shared<Model> GetLightModel() const {return m_LightModel;}
		void SetLightModel(Shared<Model> model) { m_LightModel = model; }


		Shared<Texture2D> GetShadowmap() const { return m_ShadowMap->GetDepthAttachmentTexture(); }

		LightProperties& GetLightProperties() { return m_LightProp; }
		const LightProperties GetLightProperties() const { return m_LightProp; }


	protected:
		Shared<Framebuffer> m_ShadowMap = nullptr;
		Shared<Model> m_LightModel = nullptr;
		LightProperties m_LightProp;
		glm::mat4 m_ProjMatrix{};
		glm::mat4 m_ViewMatrix{};

	};
		
}

