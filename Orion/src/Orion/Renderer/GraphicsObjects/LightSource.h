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
		struct GeneralLightProp 
		{
			glm::vec3 AmbientLightColor{ 0.01f, 0.01f, 0.01f };
			float padding0;
			glm::vec3 DiffuseLightColor{ 25.0f, 25.f, 25.0f};
			float padding1;
			glm::vec3 SpecularLightColor{ 0.5f, 0.5f, 0.5f };
			float padding2;
			glm::vec3 Position{0.0f};
			float padding3;
			glm::vec3 Direction{ 0.1f,-1.0f,0.1f };
		

		};
		struct DirectionalLightProp 
		{
			GeneralLightProp GeneralProp;

			float padding0;


			glm::mat4 ViewProj{ 1.0f };
		};

		struct PointLightProp 
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
			
			float OuterCutOff = cos(1.0f);
			float InnerCutOff = cos(0.9f);


			glm::mat4 ViewProj{1.0f};
		};
		enum class LightCasterTypes
		{
			DirectionalLight = 0,
			PointLight = 0,
			SpotLight = 0
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

		virtual LightCasterTypes GetLightType() = 0;


		const glm::mat4& GetProjectionViewMatrix() { return m_ProjMatrix * m_ViewMatrix; }
		const FrustumShape& GetFrustum() { return m_Frustum;  }
		const Shared<Model> GetLightModel() const {return m_LightModel;}
		void SetLightModel(Shared<Model> model) { m_LightModel = model;  }

		Shared<Texture2D> GetShadowmap() const { return m_ShadowMap->GetDepthAttachmentTexture(); }

	protected: 
	virtual	void UpdateFrustum() = 0;

	protected:

		Shared<Framebuffer> m_ShadowMap = nullptr;
		Shared<Model> m_LightModel = nullptr;
		glm::mat4 m_ProjMatrix{ 1.0f };
		glm::mat4 m_ViewMatrix{ 1.0f };

		FrustumShape m_Frustum;
	};
		
}

