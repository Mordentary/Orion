#pragma once
#include"Model.h"
#include"../GraphicsCore/Shader.h"
#include"../GraphicsCore/Framebuffer.h"


namespace Orion
{

	class LightHandler;

	class LightSource 
	{
	public:
		struct GeneralLightProp //60 bytes
		{
			glm::vec3 AmbientLightColor{ 0.01f, 0.01f, 0.01f };
			float padding0;
			glm::vec3 DiffuseLightColor{ 0.7f, 0.7f, 0.7f};
			float padding1;
			glm::vec3 SpecularLightColor{ 0.5f, 0.5f, 0.5f };
			float padding2;
			glm::vec3 Position{0.0f};
			float padding3;
			glm::vec3 Direction{ 0.1f,-1.0f,0.1f };
		

		};
		struct DirectionalLightProp // 124 bytes
		{
			GeneralLightProp GeneralProp;

			float padding0;


			glm::mat4 ViewProj{ 1.0f };
		};

		struct PointLightProp // 80 bytes
		{
			GeneralLightProp GeneralProp;


			float ConstantAttenuation = 1.0f;
			float LinearAttenuation = 0.065f;
			float QuadraticAttenuation = 0.1f;
			float FarPlane = 20.f;

			float Radius = 1.0f;
		
			
		};

		struct SpotLightProp // 144 bytes
		{
			GeneralLightProp GeneralProp;

			float ConstantAttenuation = 1.0f;
			float LinearAttenuation = 0.065f;
			float QuadraticAttenuation = 0.1f;
			
			float OuterCutOff = 6.0f;
			float InnerCutOff = 5.f;


			glm::mat4 ViewProj{1.0f};
		};

	public:

		LightSource() = default;
		virtual ~LightSource() = default;


		LightSource(const Shared<Model>& lightModel) : m_LightModel(lightModel)
		{
			
		}
		virtual void LoadLightToUBO(const Shared<UniformBuffer>& ubo) = 0;
		virtual void LoadLightToShader(const Shared<Shader>& shader) = 0;

		virtual void SetupLight(Shared<Shader>& currentShader, std::vector<Shared<LightSource>>& otherLights, std::function<void()> renderFunc) = 0;
		virtual void RenderLightModel(Shared<Shader>& shader) = 0;

		virtual GeneralLightProp& GetGeneralLightProp() = 0;
		virtual void SetLighAttenuation(float linear, float quadratic) = 0;
		 
		virtual void IncreaseLightCounter() = 0;
		virtual uint32_t GetLightIndex() = 0;

		const Shared<Model> GetLightModel() const {return m_LightModel;}
		void SetLightModel(Shared<Model> model) { m_LightModel = model;  }


		Shared<Texture2D> GetShadowmap() const { return m_ShadowMap->GetDepthAttachmentTexture(); }


	protected:

		Shared<Framebuffer> m_ShadowMap = nullptr;
		Shared<Model> m_LightModel = nullptr;
		glm::mat4 m_ProjMatrix{ 1.0f };
		glm::mat4 m_ViewMatrix{ 1.0f };

	};
		
}

