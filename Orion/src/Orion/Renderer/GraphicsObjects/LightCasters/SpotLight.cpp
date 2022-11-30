#include "oripch.h"
#include "SpotLight.h"

namespace Orion
{
	void SpotLight::RenderLightModel(Shared<Shader>& shader)
	{
		if (m_LightModel)
		{
			shader->Bind();
			shader->SetMat4("u_ModelMatrix", glm::translate(glm::mat4(1.0f), m_Prop.GeneralProp.Position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
			shader->SetFloat3("u_LightColor", m_Prop.GeneralProp.DiffuseLightColor);

			m_LightModel->Render(shader);
		}
	}
	void SpotLight::SetupLight(Shared<Shader>& currentShader, std::vector<Shared<LightSource>>& otherLights, std::function<void()> renderFunc)
	{
	
		m_ViewMatrix = glm::lookAt(m_Prop.GeneralProp.Position, m_Prop.GeneralProp.Position + glm::normalize(m_Prop.GeneralProp.Direction), glm::vec3(0.0, 0.0, 1.0));

		auto& depthShader = Orion::ShaderLibrary::Get("TextureDepthShader");
		depthShader->Bind();
		depthShader->SetMat4("u_ViewProjMatrix", m_ProjMatrix * m_ViewMatrix);

		currentShader = depthShader;

		m_ShadowMap->Bind();
		Orion::RenderCommand::Clear(ORI_CLEAR_DEPTH);

		for (Shared<LightSource>& light : otherLights)
		{
			if (light.get() == this) continue;
			light->RenderLightModel(depthShader);
		}

		renderFunc();

		m_ShadowMap->Unbind();

		
	}
	void SpotLight::SetLighAttenuation(float linear, float quadratic)
	{
		m_Prop.LinearAttenuation = linear;
		m_Prop.QuadraticAttenuation= quadratic;

	}

	void SpotLight::LoadLightToShader(const Shared<Shader>& shader) 
	{
		m_ShadowMap->GetDepthAttachmentTexture()->Bind(14);
		shader->SetInt("u_ShadowMapSpot", m_ShadowMap->GetDepthAttachmentTexture()->GetCurrentSlot());
	}

	void SpotLight::LoadLightToUBO(const Shared<UniformBuffer>& ubo)
	{

		m_Prop.ViewProj = m_ProjMatrix * m_ViewMatrix;

		//ORI_ASSERT(alignof(LightSource::SpotLightProp) == 16, "Inproper alignment");

		ubo->SetData(&m_Prop,  sizeof(LightSource::PointLightProp), sizeof(m_Prop));
		

	}


}